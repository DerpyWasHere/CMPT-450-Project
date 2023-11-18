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
    Addr: Can be uint_32 like in championship pred 
    What is thread pointer?
*/

#include <limits.h>
#include <inttypes.h>
#include "cpu/pred/perceptron.hh"

#define NUMBER_OF_WEIGHTS 10 
#define GHL 32 
#define THRESHOLD 128
#define WL 20 

bool *GHR;
int8_t WT[1<<NUMBER_OF_WEIGHTS][WL_1]

void Initalize_Predictor()
{
    GHR = new bool[GHL];
}

// Resetting model. Do this to change config
void Reset_Predictor()
{
    for (int i = 0; i < (1<<NUMBER_OF_WEIGHTS); i++) 
    {
        for(int j = 0; j < WL; j++)
        {
            WT[i][j] = 0;
            WT[i][j] = 0;
	    }
    }
}

int8_t Perceptron_Output()
{
    // Get 10 (NUMBER_OF_WEIGHTS) inputs from GHR and dot it with weights
    // If negative, not taken --> output -1
    // If positive, taken --> output 1
    return -1;
}

// Returns taken or not taken based on a given branch and PC. 
// Counter data + branch history are backed up in case we need to restore history/change PC.
bool
PerceptronBP::lookup(ThreadID tid, Addr branchAddr, void * &bpHistory)
{
    return 0;
}

// Called on unconditional branch instructions. Unconditional branches are always taken.
void 
PerceptronBP::uncondBranch(ThreadID tid, Addr br_pc, void* &bp_history)
{

}

// Called when there is a miss in the Branch Target buffer. Branch prediction does not know where
// to jump and thus predict not taken. 
void 
PerceptronBP::btbUpdate(ThreadID tid, Addr branch_addr, void* &bp_history)
{

}

// Update branch predictor counters. squashed implies whether update is called during a squash call.
void 
PerceptronBP::update(ThreadID tid, Addr branch_addr, bool taken, void *bp_history,
                    bool squashed, const StaticInstPtr & inst, Addr corrTarget)
{

}

// When we mispredict a branch, we revert state to before the mispredicted branch instruction was issued.
void
PerceptronBP::squash(ThreadID tid, void *bp_history)
{

}
