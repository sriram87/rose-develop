#include "sage3basic.h"
#include "analysis.h"
#include "composed_analysis.h"
#include "compose.h"
#include "printAnalysisStates.h"
#include "ats_graph_structure.h"
#include "const_prop_count.h"


using namespace std;

namespace fuse {
  ConstPropCountAnalysis::ConstPropCountAnalysis() :FWDataflow(false, false), m_assignExpCount(0) { }

  ComposedAnalysisPtr ConstPropCountAnalysis::copy() {
    return boost::make_shared<ConstPropCountAnalysis>(*this);
  }

  ValueObjectPtr ConstPropCountAnalysis::Expr2Val(SgNode* sgn, PartEdgePtr pedge) {
    assert(false);
  }

  bool ConstPropCountAnalysis::implementsExpr2Val() {
    return false;
  }

  void ConstPropCountAnalysis::genInitLattice(/*PartPtr part, PartEdgePtr pedge, PartPtr supersetPart,*/
					      const AnalysisParts& parts, const AnalysisPartEdges& pedges,
					      std::vector<Lattice*>& initLattices) {
    initLattices.push_back((Lattice*)(new BoolAndLattice(0, pedges.NodeState())));
  }
  
  bool ConstPropCountAnalysis::transfer(AnalysisParts& parts, CFGNode cn, NodeState& state, std::map<PartEdgePtr, std::vector<Lattice*> >& dfInfo) {
    set<CFGNode> nodes = parts.NodeState()->CFGNodes();
    for(set<CFGNode>::iterator n=nodes.begin(); n!=nodes.end(); n++) {
      if(SgAssignOp* assignOp = isSgAssignOp(n->getNode())) {
	ValueObjectPtr v = getComposer()->OperandExpr2Val(assignOp, assignOp->get_lhs_operand(), parts.NodeState()->inEdgeFromAny(), this);
	cout << v->str() << endl;
	if(v->isConcrete()) {
	  m_assignExpCount++;
	}
      }
    }
    PartEdgePtr accessEdge = parts.index()->inEdgeFromAny();
    return dynamic_cast<BoolAndLattice*>(dfInfo[accessEdge][0])->set(true);
  }

  void ConstPropCountAnalysis::print_stats() {
    cout << "m_assignExpCount=" << m_assignExpCount << endl;
  }

  string ConstPropCountAnalysis::str(string indent) {
    return "ConstPropCountAnalysis";
  }
    
}                                         

