# Implementation of ic3 algorithm in 2ls for program with boolean variables
This program works for programs with boolean variables and a single loop. The program state before the loop is the initial state and the loop is the transition relation.
## installation-
1> Inside summary_chaecker_base.cpp put the line in the end of SSA_functions() function like below-
````
    if(simplify)
    
    {
    
      status() << "Simplifying" << messaget::eom;
      
      ::simplify(SSA, ns);
      
    }

    SSA.output(debug()); debug() << eom;
   
  }
  
  // properties
  initialize_property_map(goto_model.goto_functions);

  CustomSSAOperation(ssa_db.get("main"), ns, "main");//----this line is added at line 83------//

}
````

2> Copy the content of new_ic3.cpp to 2ls-master/src/2ls/summary_checker_base.cpp

3> Recompile 2ls.
## Running-
Run with "2ls program_name".
