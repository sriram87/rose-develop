#pragma once
#include "latticeFull.h"
#include "staticSingleAssignment.h"
#include <boost/foreach.hpp>
#include "heapSSA.h"

#include <vector>
#include <map>

// using namespace hssa_private;
using namespace std;

#include "sight.h"
using namespace sight;

template <class LatticeType>
class SSAVariableStateTransfer : public DFTransferVisitor { 
 protected:
  bool modified;
  hssa_private::HeapSSA* ssa;
  void updateModified(bool latModified) { modified = latModified || modified; }

  // TODO: FiniteVarsExprsProductLattice* prodLat;

  map<const SgNode*, LatticeType* > latticeMap;
  map<const SgNode*, LatticeType* > tmpLatticeMap;
 
  int debugLevel;
 
  LatticeType *getLattice(const SgExpression *sgn) { 
    // std::cout << "getting lattice " << sgn->class_name() << std::endl;
    SIGHT_VERB_DECL(scope, (txt()<<"getLattice("<<SgNode2Str(sgn)<<")"), 1, debugLevel)

    // TODO: return sgn ? getLattice(SgExpr2Var(sgn)) : NULL;
    if(latticeMap.find(sgn) != latticeMap.end()) {
      return latticeMap[sgn];
    } else {
      return NULL;
    }
  };

  LatticeType *getLattice__(const SgExpression *sgn) {
    SIGHT_VERB_DECL(scope, (txt()<<"getLattice__("<<SgNode2Str(sgn)<<")"), 1, debugLevel)

    // TODO: return sgn ? getLattice(SgExpr2Var(sgn)) : NULL;       
    if (tmpLatticeMap.find(sgn) != tmpLatticeMap.end())
      return tmpLatticeMap[sgn];
    else
      return latticeMap.find(sgn) != latticeMap.end() ? latticeMap[sgn] : NULL;
  };
 
  /*** TODO: check if this is useful
  LatticeType *getLattice_(varID var) {
    // TODO: 
    return NULL;
  };*/

  void setLattice(const SgExpression *sgn, LatticeType * lattice) {
    SIGHT_VERB_DECL(scope, (txt()<<"setLattice("<<SgNode2Str(sgn)<<")"), 1, debugLevel)
    SIGHT_VERB(dbg << "lattice="<<lattice->str()<<endl, 1, debugLevel)

    if (latticeMap.find(sgn) != latticeMap.end() && lattice == NULL)
      latticeMap.erase(sgn);

    latticeMap[sgn] = lattice;
  };

  void setLattice__(const SgExpression* sgn, LatticeType * lattice) {
    SIGHT_VERB_DECL(scope, (txt()<<"setLattice__("<<SgNode2Str(sgn)<<")"), 1, debugLevel)
    SIGHT_VERB(dbg << "lattice="<<lattice->str()<<endl, 1, debugLevel)

    if (tmpLatticeMap.find(sgn) != tmpLatticeMap.end() && lattice == NULL)
      tmpLatticeMap.erase(sgn);
    
    tmpLatticeMap[sgn] = lattice;
  };

  bool getLattices(const SgBinaryOp *sgn, LatticeType* &arg1Lat, LatticeType* &arg2Lat, LatticeType* &resLat) {
    SIGHT_VERB_DECL(scope, (txt()<<"getLattice("<<SgNode2Str(sgn)<<")"), 1, debugLevel)

    arg1Lat = getLattice(sgn->get_lhs_operand());
    arg2Lat = getLattice(sgn->get_rhs_operand());
    resLat = getLattice(sgn);

    SIGHT_VERB(dbg << "arg1Lat="<<arg1Lat->str()<<endl, 1, debugLevel)
    SIGHT_VERB(dbg << "arg2Lat="<<arg2Lat->str()<<endl, 1, debugLevel)
    SIGHT_VERB(dbg << "resLat="<<resLat->str()<<endl, 1, debugLevel)

    if (isSgCompoundAssignOp(sgn)) {
      if (resLat==NULL && arg1Lat != NULL)
        resLat = arg1Lat;
    }
    
    return (arg1Lat && arg2Lat && resLat);
  };

  bool getLattices(const SgUnaryOp *sgn,  LatticeType* &arg1Lat, LatticeType* &arg2Lat, LatticeType* &resLat) {
    SIGHT_VERB_DECL(scope, (txt()<<"getLattice("<<SgNode2Str(sgn)<<")"), 1, debugLevel)

    arg1Lat = getLattice(sgn->get_operand());
    resLat = getLattice(sgn);

    SIGHT_VERB(dbg << "arg1Lat="<<arg1Lat->str()<<endl, 1, debugLevel)
    SIGHT_VERB(dbg << "resLat="<<resLat->str()<<endl, 1, debugLevel)

    // Unary Update    
    if(isSgMinusMinusOp(sgn) || isSgPlusPlusOp(sgn)) {
      arg2Lat = new LatticeType(1);
    }
    
    return (arg1Lat && arg2Lat && resLat);
  };

 public:
 SSAVariableStateTransfer(const Function& func, hssa_private::HeapSSA* ssaInstance, 
			  PartPtr p, CFGNode n, NodeState& state,
			  // const std::vector<Lattice*>& 
			  std::map<PartEdgePtr, std::vector<Lattice*> > dfInfo, const int &debugLevel_)
   : DFTransferVisitor(p, n, state, dfInfo), ssa(ssaInstance), modified(false), 
    debugLevel(debugLevel_)
    // TODO: , prodLat(dynamic_cast<FiniteVarsExprsProductLattice*>(*(dfInfo.begin())))
  {
    // Make sure that all the lattices are initialize
    // TODO: const std::vector<Lattice*>& lattices = prodLat->getLattices();
    // for(std::vector<Lattice*>::const_iterator it = lattices.begin(); it!=lattices.end(); it++)
    //   (dynamic_cast<LatticeType *>(*it))->initialize();
  }

  void visit(SgAssignOp *sgn) {
    SIGHT_VERB_DECL(scope, (txt()<<"visit(SgAssignOp)"), 1, debugLevel)

    SgExpression * lhsOperand = sgn->get_lhs_operand();
    SgExpression * rhsOperand = sgn->get_rhs_operand();
    LatticeArith * lhsLattice = getLattice(lhsOperand);
    LatticeArith * rhsLattice = getLattice(rhsOperand);

    // TODO: the lhs operand has been visited and set the lattice as its reachingDef's lattice 
    // value, this is wrong, since it is defined here
    if (rhsLattice == NULL) {
      setLattice(lhsOperand, NULL);
      return;
    }

    setLattice((const SgExpression*)lhsOperand, dynamic_cast<LatticeArith* >(rhsLattice->copy()));
    setLattice((const SgExpression*)sgn, dynamic_cast<LatticeArith* >(rhsLattice->copy()));

    /*LatticeType *lhsLat, *rhsLat, *resLat;
    getLattices(sgn, lhsLat, rhsLat, resLat);

    // Copy the lattice of the right-hand-side to both the left-hand-side variable and to the assignment expression itself    
    if(resLat) // If the left-hand-side contains a live expression or variable          
      { resLat->copy(rhsLat); modified = true; }
    if(lhsLat) // If the left-hand-side contains a live expression or variable     
      { lhsLat->copy(rhsLat); modified = true; }*/
  };

  void visit(SgAssignInitializer *sgn) {
    SIGHT_VERB_DECL(scope, (txt()<<"visit(SgAssignInitializer)"), 1, debugLevel)
    LatticeType* asgnLat = getLattice(sgn->get_operand());
    // LatticeType* resLat = getLattice(sgn);

    // If the result expression is live      
    // if (resLat) { resLat->copy(asgnLat); modified = true; }
    if (asgnLat != NULL) {
      // SgNode* node = cfgUtils::getAssignmentLHS(sgn);
      // std::cout << "LHS " << sgn->get_operand()->class_name() << std::endl;
      setLattice(sgn, dynamic_cast<LatticeArith* >(asgnLat->copy()));
    }
  };

  void visit(SgAggregateInitializer *sgn) {
    SIGHT_VERB_DECL(scope, (txt()<<"visit(SgAggregateInitializer)"), 1, debugLevel)
    LatticeType *res = getLattice(sgn);
    SgExpressionPtrList &inits = sgn->get_initializers()->get_expressions();
    if (inits.size() > 0) {
      res->copy(getLattice(inits[0]));
      modified = true;
      for (int i = 1; i < inits.size(); ++i)
        res->meetUpdate(getLattice(inits[i]));
    }
  };

  void visit(SgConstructorInitializer *sgn) {
    SIGHT_VERB_DECL(scope, (txt()<<"visit(SgConstructorInitializer)"), 1, debugLevel)
  };

  void visit(SgDesignatedInitializer *sgn) {
    SIGHT_VERB_DECL(scope, (txt()<<"visit(SgDesignatedInitializer)"), 1, debugLevel)
  };

  void visit(SgInitializedName *initName) {
    SIGHT_VERB_DECL(scope, (txt()<<"visit(SgInitializedName)"), 1, debugLevel)
    // LatticeType* varLat = getLattice(initName);
    // if(varLat) {
    if (initName->get_initializer() == NULL)
      return;

    LatticeType* initLat = getLattice(initName->get_initializer());
    // If there was no initializer, leave this in its default 'bottom' state      
    if (initLat) {
      // varLat->copy(initLat);
      setLattice((const SgExpression*)initName, dynamic_cast<LatticeArith* >(initLat->copy()));
      // modified = true;
      // }
    }
  };

  void visit(SgBinaryOp *sgn) {
    SIGHT_VERB_DECL(scope, (txt()<<"visit(SgBinaryOp)"), 1, debugLevel)
    LatticeType *lhs, *rhs, *res;
    getLattices(sgn, lhs, rhs, res);
    if (res) {
      res->copy(lhs);
      res->meetUpdate(rhs);
      modified = true;
    }
  };

  void visit(SgCompoundAssignOp *sgn) {
    SIGHT_VERB_DECL(scope, (txt()<<"visit(SgCompoundAssignOp)"), 1, debugLevel)
    LatticeType *lhs, *rhs, *res;
    getLattices(sgn, lhs, rhs, res);
    if (lhs)
      updateModified(lhs->meetUpdate(rhs));
    // Liveness of the result implies liveness of LHS      
    if (res) {
      res->copy(lhs);
      modified = true;
    }
  };

  void visit(SgCommaOpExp *sgn) {
    SIGHT_VERB_DECL(scope, (txt()<<"visit(SgCommaOpExp)"), 1, debugLevel)
    LatticeType *lhsLat, *rhsLat, *resLat;
    getLattices(sgn, lhsLat, rhsLat, resLat);

    if (resLat) {
      resLat->copy(rhsLat);
      modified = true;
    }
  };

  void visit(SgConditionalExp *sgn) {
    SIGHT_VERB_DECL(scope, (txt()<<"visit(SgConditionalExp)"), 1, debugLevel)
    LatticeType *condLat = getLattice(sgn->get_conditional_exp()),
      *trueLat = getLattice(sgn->get_true_exp()),
      *falseLat = getLattice(sgn->get_false_exp()),
      *resLat = getLattice(sgn);

    // Liveness of the result implies liveness of the input expressions     
    if (resLat) {
      resLat->copy(condLat);
      resLat->meetUpdate(trueLat);
      resLat->meetUpdate(falseLat);
      modified = true;
    }
  };

  void visit(SgScopeOp *) {
    assert(0);
  };

  void visit(SgBitComplementOp *sgn) {
    SIGHT_VERB_DECL(scope, (txt()<<"visit(SgBitComplementOp)"), 1, debugLevel)
    LatticeType *res = getLattice(sgn);
    if (res) {
      res->copy(getLattice(sgn->get_operand()));
      modified = true;
    }
  };
};
