#include "sage3basic.h"
#include "compose.h"
#include "fuseCommandParser.h"
#include "const_prop_analysis.h"
#include "dead_path_elim_analysis.h"
#include "tight_composer.h"
#include "call_context_sensitivity_analysis.h"
#include "pointsToAnalysis.h"
#include <boost/xpressive/xpressive.hpp>
#include <boost/xpressive/regex_actions.hpp>
#include <exception>
#include "sight.h"
#include "AnalysisAstAttribute.h"


using namespace std;
using namespace fuse;
using namespace sight;
using namespace boost::xpressive;

void FuseInit(int argc, char** argv) {
  // Command to set up the enviroment variable to find the binary fuseLayout
  // fuseLayout is required to run fuse
  // fuseLayout binary is at the same level as fuse in the build tree
  // When compiling fuse ROSE_PREFIX is defined as -DROSE_PREFIX="\"${top_builddir}\"" which
  // is top of the build tree
  // If fuse fails to find fuseLayout set up this environment variable appropriately. 
  setenv("SIGHT_LAYOUT_EXEC", (txt()<<ROSE_PREFIX<<"/projects/fuse/src/fuseLayout").c_str(), 1);

  string title = (txt() << "Process " << " Debug Output").c_str();
  string workdir = (txt() << "dbg").c_str();
  SightInit(argc, argv, title, workdir);
}

int main(int argc, char* argv[])
{
  FuseInit(argc, argv);
  cout << "========== S T A R T ==========\n";

  // Run the front end
  SgProject* project = frontend(argc, argv);

  printf("Frontend done\n");fflush(stdout);

  std::list<ComposedAnalysis*> scanalyses;
  std::list<ComposedAnalysis*> tcanalyses;

  // Check if the analysis sequence is described as pragmas
  sregex cmd_regex = *_s >> as_xpr("fuse") >> *_s >> (s1=+~_n);
  boost::xpressive::smatch what;
  string cmd_s;

  Rose_STL_Container<SgNode*> pragmas = NodeQuery::querySubTree(project, V_SgPragma);
  for(Rose_STL_Container<SgNode*>::iterator p=pragmas.begin(); p!=pragmas.end(); p++) {
    SgPragma* pragma = isSgPragma(*p);
    assert(pragma);

    // currently processing only one fuse command
    if(regex_match(pragma->get_pragma(), what, cmd_regex)) {
      assert(what.size() == 2);
      cmd_s.append(what[1]);
      break;
    }
  }

  if(cmd_s.length() == 0) {
    cerr << "No Fuse Command Found!" << endl;
  }

  FuseCommandParser parser;
  FuseCommand* cmd = parser(cmd_s);
  cmd->initFuseCommand();
  cmd->execute();

  list<ComposedAnalysis*> sanalyses = cmd->getSubAnalysisList();

  FuseAnnotTraversal fuseannotations(sanalyses);
  fuseannotations.traverseInputFiles(project, preorder);
  fuseannotations.printConstantCountMapStats();
  fuseannotations.printAssignOpConstantFirstAnalysisCount();

  cout << "==========  E  N  D  ==========\n";
  return 0;
}
