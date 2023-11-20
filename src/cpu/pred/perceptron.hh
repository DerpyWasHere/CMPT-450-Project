#include "base/types.hh"
#include "cpu/pred/bpred_unit.hh"
#include "params/Perceptron.hh"

#define NUMBER_OF_WEIGHTS 10 
#define GHL 32 
#define THRESHOLD 128
#define WL 20 

class PerceptronBP : public BPredUnit
{
    private:
        bool *GHR;
        int8_t WT[1<<NUMBER_OF_WEIGHTS][WL];
        uint8_t threshold = 0; // Dynamic threshold to compare aganist hard coded max in define 

        uint32_t num_of_weights;
        uint32_t ghl;
        uint32_t wl;

        uint64_t weight_hash(Addr pc, uint32_t wt_size);
        bool predict();

    public:
        PerceptronBP(const PerceptronBPParams *params);

        bool lookup(ThreadID tid, Addr branch_addr, void* &bp_history) override;
        void uncondBranch(ThreadID tid, Addr br_pc, void* &bp_history) override;
        void btbUpdate(ThreadID tid, Addr branch_addr, void* &bp_history) override;
        void update(ThreadID tid, Addr branch_addr, bool taken, void *bp_history,
                    bool squashed, const StaticInstPtr & inst,
                    Addr corrTarget) override;
        virtual void squash(ThreadID tid, void *bp_history) override;
    
};