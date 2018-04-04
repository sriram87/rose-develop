/*****************************************
 * Author: Sriram Aananthakrishnan, 2017 *
 *****************************************/
#pragma once

#include "compose.h"

namespace fuse {
  /**************************
   * ConstPropCountAnalysis *
   **************************/
  class ConstPropCountAnalysis : virtual public FWDataflow {
    std::map<PartPtr, bool> m_constMap;
    unsigned int m_assignExpCount;
  public:
    ConstPropCountAnalysis();
    ComposedAnalysisPtr copy();

    ValueObjectPtr Expr2Val(SgNode* sgn, PartEdgePtr pedge);
    bool implementsExpr2Val();

    void genInitLattice(/*PartPtr part, PartEdgePtr pedge, PartPtr supersetPart,*/
			const AnalysisParts& parts, const AnalysisPartEdges& pedges,
			std::vector<Lattice*>& initLattices);
    bool transfer(AnalysisParts& parts, CFGNode cn, NodeState& state, std::map<PartEdgePtr, std::vector<Lattice*> >& dfInfo);

    std::string str(std::string indent="");
    void print_stats();
  };
};
