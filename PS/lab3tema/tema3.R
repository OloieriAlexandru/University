# Tema 3 PS Oloieri Alexandru

#C1
vol = function(a, n)
{
  cnt = 0;
  sq = sqrt(a);
  for (i in 1:n)
  {
    u  = runif(1,-sq,sq);
    v = runif(1,-sq,sq);
    w = (u*u)+(v*v);
    if (w<=a)
      cnt = cnt + 1;
  }
  inm = (2*sq)*(2*sq);
  #cat(a," ",n," ",inm," ",sq," ",cnt,"\n");
  return (inm*cnt/n);
}
computeAns = function(a)
{
  pi = 3.141592653589793;
  rez = (pi * (a*a)) / 2;
  val = c(10000,20000,50000);
  for (n in val)
  {
    ans = vol(a, n);
    err = abs(ans - rez);
    cat("Volumul este ",ans," pentru esantionul curent\n");
    cat("Eroarea relativa pentru n=",n," este ",err,"\n");
  }
}
computeAns(2);

#C2
h = function(n)
{
  cnt = 0;
  for (i in 1:n)
  {
    x = runif(1,0,1.5);
    y = runif(1,0,1);
    if ((x-2*y<=0) & (2*x-3*y>=0) & (3*x+2*y<=6))
      cnt = cnt + 1;
    #cat(x," ",y," ",cnt,"\n");  
  }
  #cat(cnt,"\n");
  inm = 1.5 * 1;
  return (1.5 * cnt / n);
}
h(10000);

#C3

#a
integrala1 = function()
{
  n = 10000;
  #ans = ((3*sqrt(3))/8 + 13/24);
  ans = 0.8578523861716623;  
  sum = 0;
  for (i in 1:n)
  {
    x = runif(1,0,pi/3);
    s = sin(x);
    c = cos(x);
    sum = sum + s*s*s + c*c*c;
  }
  sum = sum*(pi/3) / n;
  cat("Valoarea integralei: ",ans,", rezultatul aproximat: ",sum,"\n");
  cat("Eroarea absoluta: ", abs(ans-sum),"\n");
}
integrala1();

#b
computeSecondI = function(n)
{
  sum = 0;
  for (i in 1:n)
  {
    x = rexp(1,1);
    sum = sum + (1/(2*x^2+1))/exp(-x);
  }
  sum = sum/n;
  return (sum);
}
integrala2 = function(n,k)
{
  pi = 3.141592653589793;
  ans = pi/(2*sqrt(2));
  estimate = vector();
  for (i in 1:k)
    estimate[i] = computeSecondI(n);
  sum = mean(estimate);
  cat("Valoarea integralei: ",ans,", rezultatul aproximat: ",sum,"\n");
  cat("Eroarea absoluta: ", abs(ans-sum),"\n");
}
integrala2(50000,50);

#C5
#Helper function 
goodP = function(p)
{
  pb = runif(1,0,1);
  if (pb <= p)
    return (TRUE);
  return (FALSE);
}
c5aSimulation = function(p1,p2)
{
  days = 1;
  pc = vector();
  for (i in 1:25)
    pc[i] = FALSE;
  pc[sample(1:25,1)] = TRUE;
  while (TRUE)
  {
    cnt = sum(pc);
    if (cnt == 0)
      break;
    days = days + 1;
    if (goodP(p1) && cnt < 25) # mai infectez unul in ziua curenta
    {
      now = sample(1:25,1);
      while(pc[now] == TRUE)
        now = sample(1:25,1);
      pc[now] = TRUE;
    }
    if (goodP(p2)) # curata calculatoare
    {
      perm = sample(1:25,25,replace = FALSE);
      rem = 2;
      rm = 0;
      if (sum(pc) == 1) rem = 1;
      for (i in 1:25)
      {
        if (pc[perm[i]] == FALSE) next;
        if (rm == rem) break;
        rm = rm + 1;
        pc[perm[i]] = FALSE;
      }
    }
  }
  return (days);
}
c5a = function(n, p1, p2)
{
  sum = 0;
  for (i in 1:n)
  {
    sum = sum + c5aSimulation(p1,p2);
  }
  sum = sum / n;
  return (sum);
}
c5a(10000,0.25,0.15)

c5bSimulation = function(p1,p2)
{
  pc = vector();
  for (i in 1:25)
    pc[i] = FALSE;
  pc[sample(1:25,1)] = TRUE;
  while (TRUE)
  {
    cnt = sum(pc);
    if (cnt == 0)
      break;
    if (goodP(p1) && cnt < 25) # mai infectez unul in ziua curenta
    {
      now = sample(1:25,1);
      while(pc[now] == TRUE)
        now = sample(1:25,1);
      pc[now] = TRUE;
    }
    if (sum(pc)>=5)
      return (TRUE);
    if (goodP(p2)) # curata calculatoare
    {
      perm = sample(1:25,25,replace = FALSE);
      rem = 2;
      rm = 0;
      if (sum(pc) == 1) rem = 1;
      for (i in 1:25)
      {
        if (pc[perm[i]] == FALSE) next;
        if (rm == rem) break;
        rm = rm + 1;
        pc[perm[i]] = FALSE;
      }
    }
  }
  return (FALSE);
}
c5b = function(n,p1,p2)
{
  nr = 0;
  for (i in 1:n)
  {
    if (c5bSimulation(p1,p2))
      nr = nr + 1;
  }
  nr = nr / n;
  return(nr);
}
c5b(10000,0.25,0.15);

#C7
#Helper function 

goodP = function(p)
{
  pb = runif(1,0,1);
  if (pb <= p)
    return (TRUE);
  return (FALSE);
}
c7aSimulation = function(p)
{
  pc = vector();
  once = vector();
  for (i in 1:25)
  {
    pc[i] = FALSE;
    once[i] = FALSE;
  }
  
  first = sample(1:25,1);
  pc[first] = TRUE;
  once[first] = TRUE;
  
  while (TRUE)
  {
    cnt = sum(pc);
    if (cnt == 0)
      break;
    for (i in 1:25)
    {
      if (pc[i] == TRUE)
        next;
      if (goodP(p) == TRUE) # este infectat
      {
        pc[i] = TRUE;
        once[i] = TRUE;
      }
    }
    if (sum(once) == 25)
      return (TRUE);
    
    rnd = sample(1:25, size = 25);
    nr = 0;
    for (i in 1:25)
    {
      if (pc[rnd[i]] == FALSE)
        next;
      if (nr == 4)
        break;
      nr = nr + 1;
      pc[rnd[i]] = FALSE;
    }
  }
  
  return (FALSE);
}
c7a = function(n)
{
  sum = 0;
  for (i in 1:n)
  {
    if (c7aSimulation(0.3))
      sum = sum + 1;
  }
  sum = sum / n;
  return (sum);
}
c7a(3152);

c7bSimulation = function(p)
{
  pc = vector();
  for (i in 1:25)
    pc[i] = FALSE;
  pc[sample(1:25,1)] = TRUE;
  while (TRUE)
  {
    cnt = sum(pc);
    if (cnt == 0)
      break;
    for (i in 1:25)
    {
      if (pc[i] == TRUE)
        next;
      if (goodP(p) == TRUE) # este infectat
      {
        pc[i] = TRUE;
        cnt= cnt + 1;
      }
    }
    if (cnt == 25)
      return (TRUE);
    
    rnd = sample(1:25, size = 25);
    nr = 0;
    for (i in 1:25)
    {
      if (pc[rnd[i]] == FALSE)
        next;
      if (nr == 4)
        break;
      nr = nr + 1;
      pc[rnd[i]] = FALSE;
    }
  }
  return (FALSE);
}
c7b = function(n, p)
{
  sum = 0;
  for (i in 1:n)
  {
    if (c7bSimulation(p) == TRUE)
      sum = sum + 1;
  }
  sum = sum / n;
  return (sum);
}
c7b(20,0.3)

c7c = function(epsilon, p, p_stelat)
{
  alfa = 1 - p;
  z = qnorm(alfa/2);
  nMin = p_stelat * (1 - p_stelat) * ((z/epsilon)^2);
  return (nMin);
}
c7c(0.01,0.99,0.95);

# Exercitii rezolvate, dar pe care nu le prezint
#C4

calculTimpServire = function(n) # pentru un esantion de marime n
{
  ans = 0;
  for (i in 1:n)
  {
    ansClientActual = 0;
    v1 = rgamma(1,6,4);
    v2 = rgamma(1,5,3);
    latenta = rexp(1,1);
    p = runif(1,0,1);
    if (p<=0.35){
      ansClientActual = ansClientActual + v1/1000;
    }
    else {
      ansClientActual = ansClientActual + v2/1000;
    }
    ansClientActual = ansClientActual + latenta;
    ans = ans + ansClientActual;
  }
  return (ans / n);
}
n=50000;
cat("Timpul mediu necesar servirii unui client: ",calculTimpServire(n),"\n");
cat("Aceasta valoare a fost calculata pe un esantion n=",n)
