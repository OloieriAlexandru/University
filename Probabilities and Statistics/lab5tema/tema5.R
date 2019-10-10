# Tema 5 PS Oloieri Alexandru

# D2

zConfidenceInterval = function(n,xn,nivelIncredere,disp)
{
  alpha = 1 - nivelIncredere;
  sampleMean = xn;
  sigma = sqrt(disp);
  criticalZ = qnorm(1-alpha/2,mean=0,sd=1);
  a = sampleMean - criticalZ*sigma/sqrt(n);
  b = sampleMean + criticalZ*sigma/sqrt(n);
  interval = c(a,b);
  return (interval);
}
d2 = zConfidenceInterval(196,35,0.99,16);
cat("Intervalul: [",d2[1],",",d2[2],"]\n");

# D1
d1 = zConfidenceInterval(8,140,0.95,10^2);
cat("Intervalul: [",d1[1],",",d1[2],"]\n");

# D3
testProportion = function(alpha, n, success, p0, type)
  #type = 1 -> asimetric stanga
  #type = 2 -> asimetric dreapta
  #type = 3 -> ipoteza simetrica
{
  pPrim = success / n;
  zScor = (pPrim - p0)/(sqrt(p0*(1-p0)/n));
  ok = FALSE;
  if (type == 1)
  {
    zStar = qnorm(alpha,0,1);
    if (zScor >= zStar)
      ok = TRUE;  
  }
  if (type == 2)
  {
    zStar = qnorm(1-alpha,0,1);
    if (zScor <= zStar)
      ok = TRUE;
  }
  if (type == 3)
  {
    zStar = qnorm(1-alpha/2,0,1);
    if (abs(zScor) <= abs(zStar))
      ok = TRUE;
  }
  if (ok == TRUE)
    print("Se accepta ipoteza nula, se respinge ipoteza alternativa");
  if (ok == FALSE)
    print("Se accepta ipoteza alternativa, se respinge ipoteza nula");
}
testProportion(0.01,100,32,0.3,2)
testProportion(0.05,100,32,0.3,2)

#D4
testProportion(0.01,100,48,0.509,1)
testProportion(0.05,100,48,0.509,1)