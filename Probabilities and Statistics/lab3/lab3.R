#Capitolul 1, exercitiu rezolvat
discarea = function(N){
  cnt = 0;
  for (i in 1:N){
    x = runif(1,-1,1);
    y = runif(1,-1,1);
    if (x*x + y*y <= 1){
      cnt = cnt + 1;
    }
  }
  return (4*cnt/N);
}
discarea(10000)
discarea(50000)
discarea(100000)

#Capitolul 1, exercitii propuse
ex11 = function(N){
  cnt = 0;
  for (i in 1:N){
    x = runif(1,-1,1);
    y = runif(1,-1,1);
    z = runif(1,-1,1);
    if (x*x + y*y + z*z <= 1){
      cnt = cnt + 1;
    }
  }
  return (8*cnt/N);
}
ex11(10000);
ex11(50000);
ex11(100000);

ex12 = function(N){
 cnt = 0;
 for (i in 1:N){
   x = runif(1,-1,2);
   y = runif(1,0,2);
   if (y<=(-2*(x*x)+5*x-2)){
     cnt = cnt + 1;
   }
 }
 return (9*cnt/N);
}
ex12(10000);
ex12(50000);
ex12(100000);

#Capitolul 2, exercitiu rezolvat
MC_integration = function(N) {
  sum = 0;
  for (i in 1:N){
    u = runif(1,0,10);
    sum = sum + exp((-u*u)/2);
  }
  return (10*sum/N);
}

MC_integr_average = function(k, N){
  estimates = vector();
  for (i in 1:k){
    estimates[i] = MC_integration(N);
  }
  print(mean(estimates));
  print(sd(estimates));
}
MC_integr_average(30,20000);
MC_integr_average(30,50000);

#Capitolul 2, exercitii propuse
ex21a = function(N){
  sum = 0;
  for (i in 1:N){
    u = runif(1,0,pi);
    sum = sum + sin(u) * sin(u);
  }
  return (pi*sum/N);
}
ex21a(10000);

ex21c = function(N){
  sum = 0;
  for (i in 1:N){
    u = runif(1,0,1);
    sum = sum + 1/(sqrt(1-u*u));
  }
  return (sum/N);
}
ex21c(10000);

#Capitolul 3, exericitiu rezolvat
Nr_days = function() {
  nrDays = 1;
  lastErrors = c(27,31);
  nrErrors = 27;
  while (nrErrors > 0) {
    lambda = min(lastErrors);
    nrErrors = rpois(1, lambda);
    lastErrors = c(nrErrors, lastErrors[1]);
    nrDays = nrDays + 1;
  }
  return (nrDays);
}
MC_nr_days = function(N) {
  s = 0;
  for (i in 1:N){
    s = s + Nr_days();
  }
  return (s/N);
}
MC_nr_days(10000);

#Capitolul 3, exercitii propuse
ex31Nr_days = function() {
  nrDays = 2;
  lastErrors = c(13,15,9);
  nrErrors = 13;
  while (nrErrors > 0) {
    lambda = mean(lastErrors);
    nrErrors = rpois(1, lambda);
    lastErrors = c(nrErrors,lastErrors[1],lastErrors[2]);
    nrDays = nrDays + 1;
  }
  return (nrDays);
}
ex31MC_nr_days = function(N) {
  s = 0;
  for (i in 1:N){
    s = s + ex31Nr_days();
  }
  return (s/N);
}
ex31MC_nr_days(1000);

ex32 = function(N, lambda) {
  timp = 0;
  for (i in 1:N){
    timp = timp + rexp(1,lambda);
  }
  return (timp*60/N);
}
ex32(10000,4);
ex32(10000,12);

#Capitolul 4, exercitiu rezolvat
Cap4Nr_days = function() {
  nr_days = 2;
  lastErrors = c(18, 22, 28);
  nrErrors = 18;
  while (nrErrors > 0){
    lambda = min(lastErrors);
    nrErrors = rpois(1, lambda);
    lastErrors = c(nrErrors, lastErrors[1:2]);
    nr_days = nr_days + 1;
  }
  return (nr_days);
}
MC_nr_days_21 = function(N) {
  s = 0;
  for (i in 1:N){
    if (Cap4Nr_days() > 21){
      s = s + 1;
    }
  }
  return (s/N);
}
MC_nr_days_21(5000);

#Capitolul 4, exercitii propuse
ex41a = function(N) {
  x = rgeom(N,0.3);
  y = rgeom(N,0.5);
  s = 0;
  for (i in 1:N){
    if (x[i] < (y[i]*y[i])){
      s = s + 1;
    }
  }
  return (s/N);
}
ex41a(10000);

ex41b = function() {
  alfa = 1 - 0.95;
  z = qnorm(alfa/2);
  epsilon = 0.01;
  p = 0.2841;
  Nmin = p*(1-p)*(z/epsilon)^2;
  ex41(Nmin);
  return (Nmin);
}
ex41b();




infects = function(p) {
  pb = runif(1,0,1);
  if (pb <= p){
    return (TRUE);
  }
  return (FALSE);
}

ex42help = function(N,p) {
  pc = vector();
  for (i in 1:40)
    pc[i] = FALSE;
  inf = sample(40, 1);
  pc[inf] = TRUE;
  if (infects(p)) {
    inf = sample(40,1);
    while (pc[inf] == TRUE){
      inf = sample(40,1);
    }
    pc[inf] = TRUE;
  }
  while (sum(pc) < N) {
    nrAct = sum(pc);
    #print(sum(pc));
    for (i in 1:nrAct) {
      if (sum(nrAct)>=N){
        return (TRUE);
      }
      if (infects(p)){
        inf = sample(40,1);
        while (pc[inf] == TRUE){
          inf = sample(40,1);
        }
        pc[inf] = TRUE;
      }
    }
    if (sum(nrAct)>=N){
      return (TRUE);
    }
    
    dezinf = sample(40, 10);
    for (i in 1:10){
      pc[dezinf[i]] = FALSE;
    }
    
    if (sum(pc) == 0){
      return (FALSE);
    }
  }
  return (TRUE);
}

ex42a = function(N) {
  s = 0;
  for (i in 1:N){
    if (ex42help(40,0.8) == TRUE){
      s = s+1;
    }
  }
  return (s/N);
}

ex42b = function(N) {
  s = 0;
  for (i in 1:N){
    if (ex42help(15,0.4) == TRUE){
      s = s+1;
    }
  }
  return (s/N);
}

ex42c = function() {
  alfa = 1 - 0.95;
  z = qnorm(alfa/2);
  epsilon = 0.01;
  p = 0.3147;
  Nmin = p*(1-p)*(z/epsilon)^2;
  return (Nmin);
}

ex42b(10)