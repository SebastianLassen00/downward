#ifndef PDBS_INCREMENTAL_CANONICAL_PDBS_H
#define PDBS_INCREMENTAL_CANONICAL_PDBS_H

#include "pdb_max_cliques.h"
#include "types.h"

#include "../task_proxy.h"

#include <memory>


class IncrementalCanonicalPDBs {
    const std::shared_ptr<AbstractTask> task;
    TaskProxy task_proxy;

    std::shared_ptr<PDBCollection> pattern_databases;

    // A maximal clique represents a maximal additive subset of patterns.
    std::shared_ptr<MaxAdditivePDBSubsets> max_cliques;

    // A pair of variables is additive if no operator has an effect on both.
    VariableAdditivity are_additive;

    // The sum of all abstract state sizes of all pdbs in the collection.
    int size;

    // Adds a PDB for pattern but does not recompute max_cliques.
    void add_pdb_for_pattern(const Pattern &pattern);

    void recompute_max_cliques();
public:
    explicit IncrementalCanonicalPDBs(const std::shared_ptr<AbstractTask> task,
                                      const PatternCollection &intitial_patterns);
    virtual ~IncrementalCanonicalPDBs() = default;

    // Adds a new pattern to the collection and recomputes maximal cliques.
    void add_pattern(const Pattern &pattern);

    /* Returns a set of cliques that would be additive to the new pattern.
       Detailed documentation in pdb_max_cliques.h */
    MaxAdditivePDBSubsets get_max_additive_subsets(const Pattern &new_pattern);

    // TODO issue585: the old code supported heuristic caching. Do we need this?
    int get_value(const State &state) const;

    /*
      The following method offers a quick dead-end check for the sampling
      procedure of iPDB-hillclimbing. This exists because we can much more
      efficiently test if the canonical heuristic is infinite than
      computing the exact heuristic value.
    */
    bool is_dead_end(const State &state) const;

    std::shared_ptr<PDBCollection> get_pattern_databases() const {
        return pattern_databases;
    }

    std::shared_ptr<MaxAdditivePDBSubsets> get_cliques() {
        return max_cliques;
    }

    int get_size() const {
        return size;
    }
};

#endif