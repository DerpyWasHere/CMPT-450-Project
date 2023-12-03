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

PerceptronBP::PerceptronBP(const PerceptronBPParams *params) : BPredUnit(params), 
      globalHistory(params->numThreads, 0),
      weights(params->number_of_perceptrons, 
      std::vector<int8_t>(params->number_of_weights, 0))
{
    number_of_perceptrons = params->number_of_perceptrons;
    number_of_weights = params->number_of_weights;
    global_history_bits = params->global_history_bits;
    history_mask = mask(global_history_bits);

    // Taken from section 5.1 - Tuning the predictors
    threshold = floor(1.93 * global_history_bits + 14);

    DPRINTF(Perceptron, "History mask: %ld.\n", history_mask);
}

/*
* Weight table lookup, returns modulus of pc with the number of perceptrons.
*/
uint64_t 
PerceptronBP::weight_hash(Addr pc, uint32_t num_perceptron)
{
    DPRINTF(Perceptron, "Hash for address %ld is %ld.\n", pc, pc % num_perceptron);
    return pc % num_perceptron; // Take simple mod of wt_size
}

/*
* PerceptronBP::updateGlobalHistTaken() and updateGLobalHistNotTaken are borrowed from tournament.cc and simply update the global history register.
*/

inline
void
PerceptronBP::updateGlobalHistTaken(ThreadID tid)
{
    DPRINTF(Perceptron, "Register for %ld updated with taken.\n", tid);
    
    globalHistory[tid] = (globalHistory[tid] << 1) | 1;
    globalHistory[tid] = globalHistory[tid] & history_mask;
}

inline
void
PerceptronBP::updateGlobalHistNotTaken(ThreadID tid)
{
    DPRINTF(Perceptron, "Register for %ld updated with not taken.\n", tid);
    
    globalHistory[tid] = (globalHistory[tid] << 1);
    globalHistory[tid] = globalHistory[tid] & history_mask;
}

/* 
* Returns taken or not taken based on a given branch and PC. 
* Counter data + branch history are backed up in case we need to restore history/change PC.
*/
bool
PerceptronBP::lookup(ThreadID tid, Addr branch_addr, void * &bp_history)
{
    DPRINTF(Perceptron, "Lookup performed for thread %d on address %ld.\n", tid, branch_addr);
    DPRINTF(Perceptron, "y initalized to %d with address %ld.\n", weights[perceptron_index][0], branch_addr);

    // Choose perceptron from table
    uint64_t perceptron_index = weight_hash(branch_addr, number_of_weights);
    // Get global history
    uint64_t history = globalHistory[tid];
    // Get initial prediction
    int32_t y = weights[perceptron_index][0];

    // Compute dot product
    for(uint32_t i = 1; i < global_history_bits; i++)
    {
        DPRINTF(Perceptron, "History: %ld, History shifted: %d\n", history, (history >> (i - 1)) & 1);
        if((history >> (i - 1)) & 1)  
        {
            // history positive so we take the positive weighted sum
            y += weights[perceptron_index][i]; 
        } 
        else { 
            // history negative so we take the negative weighted sum
            y -= weights[perceptron_index][i];
        } 
    }

    DPRINTF(Perceptron, "after dot product, y is now %d.\n", weights[perceptron_index][0]);
    
    // Make prediction
    bool prediction = (y >= 0) ? true : false;

    // Record history
    BPHistory *hist = new BPHistory;
    hist->globalHistory = globalHistory[tid];
    bp_history = static_cast<void*>(hist);

    // Update history
    if(prediction)
        updateGlobalHistTaken(tid);
    else updateGlobalHistNotTaken(tid);

    return prediction;
}

/*
* Called on unconditional branch instructions. Unconditional branches are always taken.
*/
void 
PerceptronBP::uncondBranch(ThreadID tid, Addr br_pc, void* &bp_history)
{
    DPRINTF(Perceptron, "uncondBranch called on thread %d with address %ld.\n", tid, br_pc);

    BPHistory *hist = new BPHistory;
    hist->globalHistory = globalHistory[tid];
    bp_history = static_cast<void*>(hist);
    updateGlobalHistTaken(tid);
}

/*
* Called when there is a miss in the Branch Target buffer. Branch prediction does not know where
* to jump and thus predicts not taken. 
*/
void 
PerceptronBP::btbUpdate(ThreadID tid, Addr branch_addr, void* &bp_history)
{
    DPRINTF(Perceptron, "btbUpdate called on thread %d with address %ld.\n", tid, branch_addr);

    BPHistory *hist = new BPHistory;
    hist->globalHistory = globalHistory[tid];
    bp_history = static_cast<void*>(hist);
    updateGlobalHistNotTaken(tid);
}

/*
* Update branch predictor counters. squashed implies whether update is called during a squash call.
* This needs to update and tweak weight training model 
*/
void 
PerceptronBP::update(ThreadID tid, Addr branch_addr, bool taken, void *bp_history,
                    bool squashed, const StaticInstPtr & inst, Addr corrTarget)
{
    DPRINTF(Perceptron, "Update called on thread %d with address %ld.\n", tid, branch_addr);
    if(taken)
        DPRINTF(Perceptron, "\tThis prediction was taken.");
    else DPRINTF(Perceptron, "\tThis prediction was not taken.");
    if(squashed)
        DPRINTF(Perceptron, "\tThis instruction was squashed.");
    else DPRINTF(Perceptron, "\tThis instruction was not squashed");

    // Cast bp_history into our branch predictor history class.
    BPHistory *history = static_cast<BPHistory *>(bp_history);

    // If squash, undo history
    if (squashed) {
        globalHistory[tid] = (history->globalHistory << 1) | taken;
        return;
    }

    // Generate prediction
    uint64_t perceptron_index = weight_hash(branch_addr, number_of_weights);
    int32_t y = weights[perceptron_index][0]; // w_0
    for(unsigned int i = 1; i < global_history_bits; i++)
    {
        if(((globalHistory[tid] >> (i - 1)) & 1) == 1)
            y += weights[perceptron_index][i]; // GHR[i] == 1 so 1
        else y -= weights[perceptron_index][i]; // GHR[i] == 0 so -1
    }

    int prediction = (y >= 0) ? 1 : -1;

    // Convert taken (bool) to sign (int)
    int sign = 0;
    if(taken) 
        sign = 1;
    else sign = -1;

    uint32_t index = weight_hash(branch_addr, number_of_weights);
    // Training algorithm taken from section 3.3 of Jimenez et al.
    if (prediction != sign || abs(y) < threshold)
    {
        // First weight always taken to be +/- 1
        if(taken)
            weights[index][0] += 1;
        else weights[index][0] -= 1;

        // Then start at w_1
        for(uint32_t i = 1; i < number_of_weights; i++)
        {
            weights[index][i] += sign;
        }
    }

    // After we finish, delete history
    delete history;
}
/*
* When we mispredict a branch, we revert state to before the mispredicted branch instruction was issued.
*/
void
PerceptronBP::squash(ThreadID tid, void *bp_history)
{
    DPRINTF(Perceptron, "Squash called on thread %d.\n", tid);

    BPHistory *history = static_cast<BPHistory *>(bp_history);
    globalHistory[tid] = history->globalHistory;

    // Delete history once finished
    delete history;
}

/*
* Creates a new PerceptronBP object with parameters generated from this parameter class.
*/
PerceptronBP*
PerceptronBPParams::create()
{
    DPRINTF(Perceptron, "PerceptronBPParams::create() called");
    return new PerceptronBP(this);
}