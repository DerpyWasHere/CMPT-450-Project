/* PAPER DATA STRUCTURES TO IMPLEMENT
    - 3.2: Perceptron
        Takes in n inputs, where each is -1 or 1. The n inputs are passed from
        a global branch history shift register.
        Also has weights which are signed ints.
        Output of perceptron is n weights dotted with n inputs, 
        with first weight (w naught) not being dotted and just added
        negative output predict not taken --> output -1 
        non-negative output predict taken --> output 1
*/

/* TRAINING ALGHORITHM 
    - 3.3: Threshold: a parameter to training algo to decide when enough training has been done 
    let t = branch result (not-taken: t = -1, taken: t = 1)
    if(signof(perceptron output) != t) or (perceptron output <= threshold) then
        for i from 1 to n
            w[i] = w[i] + t*x[i];
        end for;
    end if;
// DIRECT FROM PAPER
// "Since t and xi are always either -1 or 1, this algorithm increments the ith weight when the branch outcome agrees with
// xi , and decrements the weight when it disagrees. Intuitively, when there is mostly agreement, i.e., positive correlation, the
// weight becomes large. When there is mostly disagreement, i.e., negative correlation, the weight becomes negative with
// large magnitude"
Because the training algorithm will only change a weight when the magnitude of perceptron output is less than the threshold, no weight can exceed the value of the threshold. 
Thus, the number of bits needed to represent a weight is one (for the sign bit) plus the floor(logbase2(threshold))
*/

/* LINEAR SPERABILITY
    - 3.4: Perceptrons can only learn "linear sperable" which can divide our training algo into the set of inputs for which the perceptron will respond false
    and the set for which the perceptron will respond true
    // DIRECT FROM PAPER
    // "Boolean function over variables x1::n is linearly separable if and only if there exist values for w0::n such that all of the true
    // instances can be separated from all of the false instances by that hyperplane."
*/

/* WHOLE ALGORITHM 
    3.5:
        1. The branch address is hashed to produce an index i from 0..N-1 into the table of perceptrons.
        2. The ith perceptron is fetched from the table into a vector register, P0..n, of weights.
        3. The value of y is computed as the dot product of P and the global history register.
        4. The branch is predicted not taken when y is negative, or taken otherwise.
        5. Once the actual outcome of the branch becomes known, the training algorithm uses this outcome and the value
        of y to update the weights in P.
        6. P is written back to the ith entry in the table
*/

/* 
    Threshold size: 128 so weights can be int_8t
        8 bits total for weight: Number of bits needed for threshold: 7 bits. We need one bit for sign of weight, so 8 bits total for weights

    Global history length GHL: Paper said best lengths were 12-62, so let's start with 32
    Global history address GHR: bool pointer* in array of length GHL

    Weight table WT: 2D int8_t array stores our weights, size [2^(# of weights)][History length]
    Weight length WL: History length, 20 size is based of WL_1 in championship

    ******SEAN LOOKIE HERE*********** confirm or deny 
    Addr: Can be uint_32 like in championship pred -> deny
    What is thread pointer? -> unsure
*/

#include <limits.h>
#include <inttypes.h>
#include "cpu/pred/perceptron.hh"

PerceptronBP::PerceptronBP(const PerceptronParams *params) : BPredUnit(params)
{
    number_of_perceptrons = params->number_of_perceptrons;
    number_of_weights = params->number_of_weights;
    global_history_bits = params->global_history_bits;
    history_mask = mask(global_history_bits);
}

// Weight table lookup
// Needs to return an index from 0 to (2^wt_size) for the first index in WT 
// STEP 1 IN ALGO DETAILED IN 3.5 ABOVE 
uint64_t 
PerceptronBP::weight_hash(Addr pc, uint32_t num_perceptron)
{
    return pc % num_perceptron; // Take simple mod of wt_size
}

// Resetting model. Do this to change config
// void Reset_Predictor()
// {
//     for (int i = 0; i < (1<<number_of_weights); i++) 
//     {
//         for(int j = 0; j < WL; j++)
//         {
//             WT[i][j] = 0;
//             WT[i][j] = 0;
// 	    }
//     }
// }

// no need for this function? -- remove later
int8_t Perceptron_Output()
{
    // Get 10 (NUMBER_OF_WEIGHTS) inputs from GHR and dot it with weights
    // If negative, not taken --> output -1
    // If positive, taken --> output 1
    return -1;
}

inline
void
PerceptronBP::updateGlobalHistTaken(ThreadID tid)
{
    globalHistory[tid] = (globalHistory[tid] << 1) | 1;
    globalHistory[tid] = globalHistory[tid] & history_mask;
}

inline
void
PerceptronBP::updateGlobalHistNotTaken(ThreadID tid)
{
    globalHistory[tid] = (globalHistory[tid] << 1);
    globalHistory[tid] = globalHistory[tid] & history_mask;
}

// Returns taken or not taken based on a given branch and PC. 
// Counter data + branch history are backed up in case we need to restore history/change PC.
// STEP 2-4 IN 3.5 ALGO DETAILED ABOVE
// I could be wrong about this just but i think we only need branchAddr and threadid, weight table is global
// -S: you were right i think, p sure tid can be used to index a specific thread's weight table, see bi_mode.cc.
// Also aren't perceptron_output and lookup doing the same thing? just predicting the branch? 
// -S: lookup also has to record the history, so not quite AFAIK.
bool
PerceptronBP::lookup(ThreadID tid, Addr branch_addr, void * &bp_history)
{
    
    uint64_t perceptron_index = weight_hash(branch_addr, number_of_weights);
    bool history = globalHistory[tid];

    int32_t y = weights[perceptron_index][0];
    // STEP 2-3
    // Compute dot product
    for(unsigned int i = 0; i < number_of_weights; i++)
    {
        if(history == 1)
            y += weights[perceptron_index][i]; // GHR[i] == 1 so 1
        else y -= weights[perceptron_index][i]; // GHR[i] == 0 so -1
    }
    
    // STEP 4
    bool prediction = (y >= 0) ? true : false;

    // Record history
    BPHistory *hist = new BPHistory;
    hist->globalHistory = globalHistory[tid];
    hist->globalPredTaken = prediction;
    bp_history = (void*) hist;

    // Update history
    if(prediction)
        updateGlobalHistTaken(tid);
    else updateGlobalHistNotTaken(tid);

    return prediction;
}

// Called on unconditional branch instructions. Unconditional branches are always taken.
void 
PerceptronBP::uncondBranch(ThreadID tid, Addr br_pc, void* &bp_history)
{
    BPHistory *hist = new BPHistory;
    hist->globalHistory = globalHistory[tid];
    hist->globalPredTaken = true;
    bp_history = static_cast<void*>(hist);
    updateGlobalHistTaken(tid);
}

// Called when there is a miss in the Branch Target buffer. Branch prediction does not know where
// to jump and thus predict not taken. 
void 
PerceptronBP::btbUpdate(ThreadID tid, Addr branch_addr, void* &bp_history)
{
    //BPHistory *hist = new BPHistory;
}

// Update branch predictor counters. squashed implies whether update is called during a squash call.
// This needs to update and tweak weight training model 
void 
PerceptronBP::update(ThreadID tid, Addr branch_addr, bool taken, void *bp_history,
                    bool squashed, const StaticInstPtr & inst, Addr corrTarget)
// GHR and weight table is public so *bp_History isn't required i dont think 
{
    // if sign(yout) != t or |yout| < THRESHOLD then
        // for i := 0 to n do
        // wi := wi + txi
        // end for
    // end if
    unsigned int t = 0;
    if(taken) t = 1;
    else t = -1;

    uint32_t index = weight_hash(branch_addr, number_of_weights);

    if (lookup(tid, branch_addr, bp_history) != t || lookup(tid, branch_addr, bp_history) < threshold)
    {
        for(unsigned int i = 0; i < number_of_weights; i++)
        {
            weights[index][i] += t*weights[index][i];
        }
    }
}

// When we mispredict a branch, we revert state to before the mispredicted branch instruction was issued.
void
PerceptronBP::squash(ThreadID tid, void *bp_history)
{

}

PerceptronBP*
PerceptronParams::create()
{
    return new PerceptronBP(this);
}