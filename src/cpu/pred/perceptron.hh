#include "base/bitfield.hh"
#include "base/circular_queue.hh"
#include "base/types.hh"
#include "cpu/pred/bpred_unit.hh"
#include "params/Perceptron.hh"

class PerceptronBP : public BPredUnit
{
    private:
        //bool *GHR;
        //int8_t WT[1<<NUMBER_OF_WEIGHTS][WL];
        uint8_t threshold = 0; // Dynamic threshold to compare aganist hard coded max in define 
        
        std::vector<uint64_t> globalHistory;
        std::vector<std::vector<int8_t>> weights;

        uint32_t number_of_weights;
        uint32_t number_of_perceptrons;
        uint32_t global_history_bits;
        uint64_t history_mask; // max 64 bits
        uint32_t wl;

        uint64_t weight_hash(Addr pc, uint32_t num_perceptrons);
        bool predict();

        /** Updates global history as taken. */
        inline void updateGlobalHistTaken(ThreadID tid);

        /** Updates global history as not taken. */
        inline void updateGlobalHistNotTaken(ThreadID tid);

    public:
        struct BPHistory {
            unsigned globalHistory;
            bool globalPredTaken;
        };

        PerceptronBP(const PerceptronParams *params);

        bool lookup(ThreadID tid, Addr branch_addr, void* &bp_history) override;
        void uncondBranch(ThreadID tid, Addr br_pc, void* &bp_history) override;
        void btbUpdate(ThreadID tid, Addr branch_addr, void* &bp_history) override;
        void update(ThreadID tid, Addr branch_addr, bool taken, void *bp_history,
                    bool squashed, const StaticInstPtr & inst,
                    Addr corrTarget) override;
        virtual void squash(ThreadID tid, void *bp_history) override;
    
};