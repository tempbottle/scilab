 function [P,Q]=of(A,B,C)
// Copyright INRIA
// Generated by lmitool on Thu Feb 09 11:22:01 MET 1995

   Mbound = 1e3;
   abstol = 1e-10;
   nu = 10;
   maxiters = 100;
   reltol = 1e-10;
   options=[Mbound,abstol,nu,maxiters,reltol];

 ///////////DEFINE INITIAL GUESS AND PRELIMINARY CALCULATIONS BELOW
 P_init=zeros(A)
 Q_init=zeros(A)
 NB=kernel(B');
 NC=kernel(C);
 I=eye(A);
 ///////////

 XLIST0=list(P_init,Q_init)
 XLIST=lmisolver(XLIST0,of_eval,options)
 [P,Q]=XLIST(:)



 /////////////////EVALUATION FUNCTION////////////////////////////

 function [LME,LMI,OBJ]=of_eval(XLIST)
 [P,Q]=XLIST(:)

 /////////////////DEFINE LME, LMI and OBJ BELOW
 LME=list(P-P',Q-Q')
 LMI= list(-NB'*(A*Q+Q*A'+Q)*NB,-NC'*(A'*P+P*A+P)*NC,[P I; I Q]);
 OBJ=trace(P+Q);

 endfunction
 endfunction