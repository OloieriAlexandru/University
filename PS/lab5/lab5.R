# I
# Exercitiu rezolvat

normalDensityExample = function(limit)
{
  t = seq(-limit, limit, length = 400);
  f = 1/sqrt(2*pi)*exp(-t^2/2);
  plot(t,f,type="l",lwd=1);
}
normalDensityExample(6);

# Exercitiu propus

normalDensity = function(limit, m, disp)
{
  sigma = sqrt(disp);
  t = seq(-limit, limit, length = 400);
  f = (1/(sigma*sqrt(2*pi)))*exp(-(((t-m)^2)/(2*disp)));
  plot(t, f, type="l", lwd = 1);
}
normalDensity(6, 0, 1);

normalDensity2 = function(sample)
{
  m = mean(sample);
  d = var(sample);
  sigma = sqrt(d);
  f = (1/(sigma*sqrt(2*pi)))*exp(-(((sample-m)^2)/2*d));
  plot(sample,f,type="l",lwd=1);
}
normalDens = function(fileName)
{
  x = scan(fileName);
  normalDensity2(x);
}
normalDens("history.txt")

# II
# Exercitiu propus

selectionMean = function(fileName)
{
  x = scan(fileName);
  m = mean(x);
  return (m);
}
selectionMean("history.txt")

# III
# Exercitii propuse

# III.1
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
zConfidenceInterval(100,20,0.90,9);

# III.2
zConfidenceInterval(25,67.53,0.90,100);

# III.3
zConfidenceInterval(50,5,0.95,0.5*0.5);

# III.4
zConfidenceInterval(100,1280,0.99,140*140);

# III.6
zConfidenceIntervalFile = function(fileName, nivelIncredere)
{
  x = scan(fileName);
  m = mean(x);
  d = var(x);
  n = length(x);
  return (zConfidenceInterval(n,m,nivelIncredere,m));
}
zConfidenceIntervalFile("history.txt",0.95);

# IV
# Exercitii propuse

# IV.1
tConfidenceInterval = function(n,m,nivelIncredere,disp)
{
  alpha = 1 - nivelIncredere;
  sampleMean = m;
  s = disp;
  se = s/sqrt(n);
  criticalT = qt(1-alpha/2,n-1);
  a = sampleMean - criticalT*se;
  b = sampleMean + criticalT*se;
  interval = c(a,b);
  return (interval);
}
tConfidenceInterval(60,3.3,0.95,0.4);

# IV.2
tConfidenceInterval(196,44.65,0.99,sqrt(2.25));

# IV.4
tConfidenceIntervalFile = function(fileName, nivelIncredere)
{
  x = scan(fileName);
  m = mean(x);
  s = var(x);
  n = length(x);
  return (tConfidenceInterval(n,m,nivelIncredere,s));
}
tConfidenceIntervalFile("history.txt",0.90);

# IV.5
computeTrustInt = function(vec, nivelIncredere)
{
  return (tConfidenceInterval(length(vec),mean(vec),nivelIncredere,var(vec)));
}
sample45 = c(12,11,12,10,11,12,13,12,11,11,13,14,10);
computeTrustInt(0.90);
computeTrustInt(0.95);
computeTrustInt(0.99)

# V
# Exercitiu rezolvat

p0 = 0.6;
n = 100;
success = 63;
alpha = 0.01;
p = 0.6; # Ipoteza nula
# Ha p < p0 = 0.6 (as stanga)
#3 alpha = 0.01
pPrim = success/n;
zScore = (pPrim-p0)/(sqrt((p0*(1-p0))/n))
zScore

zStar = qnorm(1-alpha,0,1);
zStar

# se accepta
# Cu o incredere de 99% se poate aproxima ca politicianul primeste aprox 60% din voturi, deci afirmatia lui se poate respinge

# Exercitii propuse
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
    if (zScor > zStar)
    {
      print("Se accepta");
      ok = TRUE;  
    }
  }
  else
  if (type == 2)
  {
    zStar = qnorm(1-alpha,0,1);
    print(zScor);
    print(zStar);
    if (zScor < zStar)
    {
      print("Se accepta");
      ok = TRUE;
    }
  }
  else
  {
    zStar = qnorm(1-alpha/2,0,1);
    if (abs(zScor) < abs(zStar))
    {
      print("Se accepta");
      ok = TRUE;
    }
  }
  if (ok == FALSE)
  {
    print("Se respinge");
  }
}
testProportion(0.01, 42, 17, 0.25, 2)

testProportion(0.01, 100, 63,0.6, 1)

# V.2
testProportion(0.05, 150, 20, 0.1, 2)

# V.3
testProportion(0.01, 42, 17, 0.25, 2)