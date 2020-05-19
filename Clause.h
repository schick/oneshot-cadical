//
// Created by maxi on 14.05.20.
//

#ifndef ONESHOT_CADICAL_CLAUSE_H
#define ONESHOT_CADICAL_CLAUSE_H


#include <vector>
#include <algorithm>
#include <cassert>

class Clause {

public:
    explicit Clause(std::vector<int> &literals, int glue) : lits{literals}, glue{glue} {
        assert(!lits.empty());
        std::sort(lits.begin(), lits.end());
    }

    explicit Clause(int unit_lit) : lits{unit_lit}, glue{0} {
        assert(!lits.empty());
    }

    bool operator<(Clause const &right) const {
        if (lits.size() == right.lits.size()) {
            for (size_t i = 0; i < lits.size(); i++)
                if (lits.at(i) != right.lits.at(i))
                    return lits.at(i) < right.lits.at(i);
            // Identical clause
            return glue < right.glue;
        }
        return lits.size() < right.lits.size();
    }

private:
    std::vector<int> lits;
    int glue;
};


#endif //ONESHOT_CADICAL_CLAUSE_H
