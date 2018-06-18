/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

std::string extract_var(std::string in)
{
    std::string::iterator i;
    std::string s="";
    for(i=in.begin();*i!='#';i++)
    {
        s.append(1,*i);
    }
    return s;
}

int is_var(std::vector<exprt> vars,std::string in)
{
    std::string var;
    if(in.at(0)=='$')
        return -1;
    else
    {
        std::string::iterator i;
        int count=-1,pos=-1;
        for(i=in.begin();i!=in.end();i++)
        {
            if(*i=='#')
            {
                std::vector<exprt>::iterator i1;
                for(i1=vars.begin();i1!=vars.end();i1++)
                {
		    std::string var1=extract_var(from_expr(*i1));
                    count++;
                    if(var.compare(var1)==0) pos=count;
                }
                if(pos==-1) return pos;
            }
            var.append(1,*i);
        }
        return pos;
    }
}
bool construct_cnf(std::vector<std::string>& P,std::ofstream& out,std::string in,std::vector<exprt> pre_vars,unsigned loophead)//reads the input formula and construct an cnf object
{
    int flag=0,sign=0,size=0;
    std::string::iterator i;
    std::string *s=new std::string(),*clause=new std::string(pre_vars.size(),'X');
    for (i=in.begin();i!=in.end();i++)
    {
        if(*i=='&')
        {
	    std::vector<exprt>::iterator i1;
            if((s->compare("True")==0&&sign==0)||(s->compare("False")==0&&sign==1))
	    {
		s->clear();
		clause->insert(0,'X',pre_vars.size());
		size=0;
		continue;
	    }
	    else if((s->compare("False")==0&&sign==0)||(s->compare("True")==0&&sign==1))
	    {
		if(size==0)
		{
		    P.clear();
		    out<<"P is FALSE.\n";
		    return false;
		}
	    }
	    else
	    {
		std::vector<exprt>::iterator i1;
		int pos=0;
		*s=*s+"#phi"+std::to_string(loophead);
                for(i1=pre_vars.begin();i1!=pre_vars.end();i1++)
                {
                    if(from_expr(*i1).compare(*s)==0) 
		    {
			if(sign==0 && clause->at(pos)!='F') (*clause)[pos]='T';
			else if(clause->at(pos)!='T') (*clause)[pos]='F';
			else
			{
			    out<<"Contradiction in I.\n";
			    return false;
			}
			break;
		    }
		    pos++;
                }
		sign=0;
		if(pos==pre_vars.size()) return false;
		P.push_back(*clause);
		s->clear();
		clause=new std::string(pre_vars.size(),'X');
		size=0;
	    }
        }
        else if(*i=='|')
        {
            std::vector<exprt>::iterator i1;
            if((s->compare("True")==0&&sign==0)||(s->compare("False")==0&&sign==1))
	    {
		while(*i!='&'||i!=in.end())
		{
		    i++;
		}
		i--;
		clause->insert(0,'X',pre_vars.size());
		size=0;
	    }
	    else if((s->compare("False")==0&&sign==0)||(s->compare("True")==0&&sign==1))
	    {
		
	    }
	    else
	    {
		std::vector<exprt>::iterator i1;
		int pos=0;
		*s=*s+"#phi"+std::to_string(loophead);
                for(i1=pre_vars.begin();i1!=pre_vars.end();i1++)
                {
                    if(from_expr(*i1).compare(*s)==0) 
		    {
			if(sign==0 && clause->at(pos)!='F') (*clause)[pos]='T';
			else if(clause->at(pos)!='T') (*clause)[pos]='F';
			else
			{
			    out<<"Contradiction in I.\n";
			    return false;
			}
			break;
		    }
		    pos++;
                }
		sign=0;
		if(pos==pre_vars.size()) return false;
	    }
	    s->clear();
	}
	else if(*i == '(')
	{
	    if(!(s->empty()))
	    {
		out<<"Error in the formula\n";
		return false;
	    }
	    if(flag==1)
	    {
		out<<"Parenthesis mismatch\n";
		return false;
	    }
	    flag=1;
	}
	else if(*i == ')')
	{
	    if(flag==0)
	    {
		out<<"Parenthesis mismatch\n";
		return false;
	    }
	    flag=0;
	}
        else if(*i == '~') sign=1;
	else
	{
	    s->append(1,*i);
	}
    }
    if(flag!=0){
	out<<"Parenthesis mismatch\n";
	return false;
    }
    std::vector<exprt>::iterator i1;
    if((s->compare("True")==0&&sign==0)||(s->compare("False")==0&&sign==1))
    {

    }
    else if((s->compare("False")==0&&sign==0)||(s->compare("True")==0&&sign==1))
    {
	if(size==0)
	{
	    P.clear();
	    out<<"P is FALSE.\n";
	    return false;
	}
    }
    else
    {
	std::vector<exprt>::iterator i1;
	int pos=0;
	*s=*s+"#phi"+std::to_string(loophead);
	for(i1=pre_vars.begin();i1!=pre_vars.end();i1++)
	{
	    if(from_expr(*i1).compare(*s)==0) 
	    {
		if(sign==0 && clause->at(pos)!='F') (*clause)[pos]='T';
		else if(clause->at(pos)!='T') (*clause)[pos]='F';
		else
		{
		    out<<"Contradiction in I.\n";
		    return false;
		}   
		break;
	    }
	    pos++;
	}
	if(pos==pre_vars.size()) return false;
	P.push_back(*clause);
    }
    if(P.empty())
    {
	clause->clear();
	out<<"Property is True.\n";
	P.clear();
	return false;
    }
    return true;
}

exprt clause_to_exprt(std::string clause,std::vector<exprt> vars)
{
    std::vector<exprt> lits;
    unsigned i2;
    constant_exprt f("00000000",vars.at(0).type());
    for(i2 = 0;i2<clause.size();i2++)
    {
        //std::cout<<clause.size()<<std::endl;
        if(clause.at(i2)=='F')
        {
	    lits.push_back(equal_exprt(vars.at(i2),f));
	}
	else if(clause.at(i2)=='T')
	{
            lits.push_back(notequal_exprt(vars.at(i2),f));
	}
    }
    return disjunction(lits);
}
std::vector<exprt> to_exprt_vec(std::vector<std::string> P,std::vector<exprt> vars)//adds clauses of the caller cnf to the Solver object
{
    std::vector<exprt> conjuncts;
    std::vector< std::string >::iterator i1;
    for(i1 = P.begin();i1 != P.end();i1++) 
    {
	conjuncts.push_back(clause_to_exprt(*i1,vars));
	//std::cout<<"\n";
    }
    //std::cout<<"solver::"<<from_expr(conjunction(conjuncts));
    return conjuncts;
}

bool subsumes(std::string cls1,std::string cls2)
{
    int i,cls_size=cls1.length();
    for(i=0;i<cls_size;i++)
    {
	if(cls1.at(i)!='X' && cls1.at(i)!=cls2.at(i)) return false;
    }
    return true;
}

bool block_recursively(std::ofstream& out,exprt I,exprt T,exprt CTI,std::vector<std::vector<std::string>> &Frames,int no,std::vector<exprt> pre_vars,std::vector<exprt> post_vars,const namespacet &ns,unsigned loophead)
{
    constant_exprt f("00000000",pre_vars.at(0).type());
    incremental_solvert solver(ns),solver1(ns);
    solver<<conjunction(to_exprt_vec(Frames.at(no),pre_vars));
    solver<<T;
    solver<<CTI;
    int count=1;
    while(solver()==D_SATISFIABLE)
    {
	std::string blocked_clause(pre_vars.size(),'X');
	std::vector<exprt> conjuncts,disjuncts;
        std::vector<exprt>::iterator iter;
	out<<"At frame "<<no+1<<"==>\n";
	int pos=0;
        for(iter=pre_vars.begin();iter!=pre_vars.end();iter++)
        {
            exprt e1=solver.get(*iter);
            if(from_expr(ns,"",e1).compare("TRUE")==0)
            {
		disjuncts.push_back(equal_exprt(*iter,f));
		conjuncts.push_back(equal_exprt(post_vars.at(pos),e1));
                blocked_clause[pos]='F';
            }
            else if(from_expr(ns,"",e1).compare("FALSE")==0)
            {
		disjuncts.push_back(notequal_exprt(*iter,f));
		conjuncts.push_back(equal_exprt(post_vars.at(pos),e1));
                blocked_clause[pos]='T';
            }
	    pos++;
        }
	out<<"  CTI "<<count<<"::"<<from_expr(conjunction(conjuncts))<<"\n";
        solver<<disjunction(disjuncts);
        if(no==0)
        {
	    incremental_solvert solver1(ns);
            solver1<<I;
            solver1<<T;
            solver1<<conjunction(conjuncts);
            if(solver1()==D_SATISFIABLE)
            {
                out<<"Bad state is reachable from initial state\n";
                return false;
            }
        }
        else
        {
            if(!(block_recursively(out,I,T,conjunction(conjuncts),Frames,no-1,pre_vars,post_vars,ns,loophead)))
            {
                return false;
            }
        }
	int i;
        for(i=0;i<=no;i++)
        {
	    bool subsumes_flag=false;
	    for(std::vector<std::string>::iterator iter1=Frames.at(i).begin();iter1!=Frames.at(i).end();iter1++)
	    {
		if(subsumes(*iter1,blocked_clause)) subsumes_flag=true;
	    }
	    if(!subsumes_flag) Frames.at(i).push_back(blocked_clause);
        }
	out<<"Blocked clause(Blocked upto frame "<<no+1<<"):: "<<from_expr(disjunction(disjuncts))<<"\n\n";
	count++;
    }
    return true;
}

/*bool is_equivalent(std::vector<std::string> frame1,std::vector<std::string> frame2)
{
    
}*/

bool propagate_clauses(std::ofstream& out,exprt T,std::vector<std::vector<std::string>> &Frames,std::vector<exprt> pre_vars,std::vector<exprt> post_vars,int framesize,const namespacet &ns)
{
    out<<"Propagation phase |-------------------------------------------->\n\n";
    int i1;
    for(i1=0;i1<=framesize;i1++)
    {
        std::vector<std::string> Clauses1=Frames.at(i1),Clauses2=Frames.at(i1+1);
	unsigned i2,i3;
        for(i2=0;i2<Clauses1.size();i2++)
        {
            bool flag=false;
            for(i3=0;i3<Clauses2.size();i3++)
            {
                if(!Clauses1.at(i2).compare(Clauses2.at(i3)))
                {
                    flag=true;
                }
            }
            if(!flag)
            {
                incremental_solvert solver(ns);
                solver<<conjunction(to_exprt_vec(Frames.at(i1),pre_vars));
		solver<<T;
                solver<<not_exprt(clause_to_exprt(Clauses1.at(i2),post_vars));
                if(solver()==D_UNSATISFIABLE)
                {
		    out<<"Propagating clause "<<from_expr(clause_to_exprt(Clauses1.at(i2),post_vars));
		    out<<" from Frame "<<i1+1<<" to Frame "<<i1+2<<"\n";
                    Frames.at(i1+1).push_back(Clauses1.at(i2));
                }
            }
        }
	out<<"\n";
    }
    if(Frames.at(framesize).size()==Frames.at(framesize+1).size())//is_equilvalent(Frames.at(framesize),Frames.at(framesize+1))
    {
        out<<"Frame "<<i1+1<<" and "<<"Frame "<<i1+2<<" are same.\n\nInvariant found!\n";
        return true;
    }
    out<<"After propagation phase::\n";
    return false;
}

void do_ic3(std::ofstream& out,local_SSAt &SSA, const namespacet &ns, const dstring name,exprt T,exprt I,std::vector<exprt> P_pre,std::vector<std::string> P,std::vector<exprt> pre_vars,std::vector<exprt> post_vars,unsigned loophead) 
{
    incremental_solvert solver(ns);
    solver<<I;
    solver<<not_exprt(conjunction(P_pre));
    if(solver()==D_SATISFIABLE)
    {
        out<<"Initial state violates the property\n";
        return;
    }
    incremental_solvert solver1(ns),solver2(ns);
    std::vector<exprt> P_post=to_exprt_vec(P,post_vars);
    solver1<<I;
    solver1<<T;
    solver1<<not_exprt(conjunction(P_post));
    if(solver1()==D_SATISFIABLE)
    {
        out<<"Bad state is reachable from initial state in one step\n";
        return;
    }
    std::vector<std::vector<std::string>> Frames;
    Frames.push_back(std::vector<std::string>(P));
    int iter=0;
    out<<"\n=======================IC3 iteration "<<iter<<"=============================="<<"\n\n";
    /*if(!(block_recursively(out,I,T,P,Frames,iter,pre_vars,post_vars,ns,loophead)))
    {
        return;
    }*/
    while(iter>=0)
    {
        incremental_solvert solver2(ns);
	solver2<<conjunction(to_exprt_vec(Frames.at(iter),pre_vars));
	solver2<<T;
	solver2<<not_exprt(conjunction(P_post));
	if(solver2()==D_SATISFIABLE)
	{
	    out<<"Blocking phase |-------------------------------------------->\n\n";
	    for(std::vector<exprt>::iterator i=P_post.begin();i!=P_post.end();i++)
	    {
		if(!(block_recursively(out,I,T,not_exprt(*i),Frames,iter,pre_vars,post_vars,ns,loophead)))
		{
		    return;
		}
	    }
	}
        else
        {
            Frames.push_back(std::vector<std::string>());
            if(propagate_clauses(out,T,Frames,pre_vars,post_vars,iter,ns)) return;
	    int i=0;
	    while(i<=iter+1)
	    {
		out<<"Frame"<<i<<"::  ";
		out<<from_expr(conjunction(to_exprt_vec(Frames.at(i),pre_vars)))<<"\n";
		i++;
	    }
            iter++;
	    out<<"\n=======================IC3 iteration "<<iter<<"=============================="<<"\n\n";
        }
    }
}

bool get_I_and_T(std::ofstream& out,local_SSAt &SSA,const namespacet &ns,std::vector<exprt>& pre_vars,std::vector<exprt>& post_vars,unsigned& loophead,exprt& I,exprt& T)
{
    std::vector<exprt> conjuncts,conjuncts1;
    unsigned loopend=0;
    loophead=0;
    for(local_SSAt::nodest::const_iterator n_it=SSA.nodes.begin();
      n_it!=SSA.nodes.end(); n_it++)
    {
        if(n_it->loophead!=SSA.nodes.end())
        {
            loophead=n_it->loophead->location->location_number;
            loopend=n_it->location->location_number;
            break;
        }
    }
    if(loophead==loopend&&loopend==0)
    {
        out<<"No loop in the program\n";
        return false;
    }
    for(local_SSAt::nodest::const_iterator n_it=SSA.nodes.begin();
      n_it!=SSA.nodes.end(); n_it++)
    {
        if(n_it->location->location_number==loophead)
        {
            exprt loop_exit_cond,temp_exp;
            for(local_SSAt::nodet::equalitiest::const_iterator c_it=n_it->equalities.begin();
              c_it!=n_it->equalities.end();
              c_it++)
            {
                pre_vars.push_back((*c_it).op0());
                loop_exit_cond=temp_exp;
                temp_exp=*c_it;
            }
            pre_vars.pop_back();
            pre_vars.pop_back();
            conjuncts.push_back(loop_exit_cond);
	    conjuncts.push_back(equal_exprt(temp_exp.op0(),true_exprt()));
        }
    }
    exprt post_vars1[pre_vars.size()];
    for(local_SSAt::nodest::const_iterator n_it=SSA.nodes.begin();
      n_it!=SSA.nodes.end(); n_it++)
    {
	if(n_it->location->location_number<loophead)
	{
	    for(local_SSAt::nodet::equalitiest::const_iterator c_it=n_it->equalities.begin();
              c_it!=n_it->equalities.end();
              c_it++)
            {
		conjuncts1.push_back(*c_it);
	    }
	}
	else if(n_it->location->location_number==loophead)
	{
	    for(local_SSAt::nodet::equalitiest::const_iterator c_it=n_it->equalities.begin();
              c_it!=n_it->equalities.end();
              c_it++)
            {
		conjuncts1.push_back(*c_it);
	    }
	    local_SSAt::nodet::equalitiest::const_iterator c_it=n_it->equalities.begin();
	    conjuncts1.pop_back();
	    conjuncts1.pop_back();
	    conjuncts1.push_back(equal_exprt(c_it->op1().op0(),false_exprt()));
	    I=conjunction(conjuncts1);
	}
        else if(n_it->location->location_number>loophead && n_it->location->location_number<=loopend)
        {
            for(local_SSAt::nodet::equalitiest::const_iterator c_it=n_it->equalities.begin();
              c_it!=n_it->equalities.end();
              c_it++)
            {
                int pos=is_var(pre_vars,from_expr(c_it->op0()));
                if(pos!=-1)
                {
                    post_vars1[pos]=c_it->op0();
                }
                conjuncts.push_back(*c_it);
            }
        }
    }
    unsigned j;
    for(j=0;j<pre_vars.size();j++)
    {
	symbol_exprt post_var(dstring(extract_var(from_expr(pre_vars.at(j)))+"#0"),pre_vars.at(0).type());
        conjuncts.push_back(equal_exprt(post_vars1[j],post_var));
	post_vars.push_back(post_var);
    }
    T=conjunction(conjuncts);
    return true;
}

void CustomSSAOperation(local_SSAt &SSA, const namespacet &ns, const dstring name)
{
    std::ofstream out("output.txt");
    out<<"//////////////////////Output of custom SSA Operation\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n\n\n";
    std::vector<exprt> pre_vars,post_vars;
    unsigned loophead=0;
    exprt I,T;
    if(!get_I_and_T(out,SSA,ns,pre_vars,post_vars,loophead,I,T)) return;
    out<<"T:= "<<from_expr(ns," ",T)<<std::endl;
    out<<"\nI:= "<<from_expr(ns," ",I)<<std::endl;
    
    std::cout<<"Give the property in CNF form(don't use space in the formula):\n";
    std::string prop;
    std::cin>>prop;
    std::vector<std::string> P;
    if(!construct_cnf(P,out,prop,pre_vars,loophead))
    {
        out<<"Error in the given property\n";
        return;
    }
    std::vector<exprt> P_pre=to_exprt_vec(P,pre_vars);
    out<<"\nP:= "<<from_expr(conjunction(P_pre))<<"\n\n";
    
    /*local_SSAt::nodest::const_iterator n_it=SSA.nodes.begin();
    n_it++;
    n_it++;
    n_it++;
    local_SSAt::nodet::equalitiest::const_iterator c_it=n_it->equalities.begin();
    exprt t=c_it->op1();
    n_it++;
    n_it++;
    c_it=n_it->equalities.begin();
    exprt f=c_it->op1();
    //constant_exprt t("00000001",pre_vars.at(0).type()),f("00000000",pre_vars.at(0).type());
    incremental_solvert solver(ns);
    //solver<<P.toexprt(0,pre_vars);
    //solver<<T;
    //solver<<P.toexprt(1,post_vars);
    //out<<from_expr(and_exprt(notequal_exprt(pre_vars.at(0),t),notequal_exprt(pre_vars.at(1),f)));
    solver<<and_exprt(equal_exprt(pre_vars.at(0),f),equal_exprt(pre_vars.at(0),f));
    //solver<<not_exprt(or_exprt(equal_exprt(post_vars.at(0),f),equal_exprt(post_vars.at(1),f)));
    //solver<<*c_it;
    //solver<<not_exprt(equal_exprt(c_it->op0(),t));
    if(solver()==D_SATISFIABLE)
    {
	out<<"SAT";
	out<<from_expr(pre_vars.at(0))<<from_expr(solver.get(pre_vars.at(0)))<<"\n";
	//out<<from_expr(pre_vars.at(1))<<from_expr(solver.get(pre_vars.at(1)))<<"\n";
	for(local_SSAt::nodest::const_iterator n_it=SSA.nodes.begin();
      n_it!=SSA.nodes.end(); n_it++)
	{
	    for(local_SSAt::nodet::equalitiest::const_iterator c_it=n_it->equalities.begin();
              c_it!=n_it->equalities.end();
              c_it++)
            {
		print_symbol_values(SSA,solver,out,*c_it);
	    }
	}
	incremental_solvert solver1(ns);
	solver1<<equal_exprt(solver.get(pre_vars.at(0)),true_exprt());
	//out<<from_expr(post_vars.at(0))<<from_expr(solver.get(post_vars.at(0)))<<"\n";
	//out<<from_expr(post_vars.at(1))<<from_expr(solver.get(post_vars.at(1)))<<"\n";
    }
    else out<<"UNSAT";*/
    
    do_ic3(out,SSA,ns,name,T,I,P_pre,P,pre_vars,post_vars,loophead);
    out.close();
}