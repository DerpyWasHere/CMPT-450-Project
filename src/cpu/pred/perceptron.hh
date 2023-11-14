#include "base/types.hh"
#include "cpu/pred/bpred_unit.hh"

class PerceptronBP : public BPredUnit
{

    public:
        bool lookup(ThreadID tid, Addr branch_addr, void* &bp_history) override;
        void uncondBranch(ThreadID tid, Addr br_pc, void* &bp_history) override;
        void btbUpdate(ThreadID tid, Addr branch_addr, void* &bp_history) override;
        void update(ThreadID tid, Addr branch_addr, bool taken, void *bp_history,
                    bool squashed, const StaticInstPtr & inst,
                    Addr corrTarget) override;
        virtual void squash(ThreadID tid, void *bp_history) override;
    
};