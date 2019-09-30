# Tema 4 PS Oloieri Alexandru

# B2

getPrimes = function(maxV)
{
  cnt = 1;
  ans = vector();
  pr = vector(mode = "logical", length = maxV);
  for (i in 2:maxV)
    if (pr[i] == FALSE)
    {
      ans[cnt] = i;
      cnt = cnt + 1;
      j = i+i;
      while (j<=maxV)
      {
        pr[j] = TRUE;
        j = j + i;
      }
    }
  return (ans);
}

number = function(sir, start, len, prime)
{
  ans = 0;
  upTo = start + len - 1;
  pow2 = 1;
  for (i in upTo : start)
  {
    act = substr(sir, i, i);
    if (act == 1)
      ans = (ans + pow2) %% prime;
     pow2 = ( pow2 * 2 ) %% prime;
  }
  return (ans);
}

isSubstring = function(a, b)
{
  n = nchar(a);
  m = nchar(b);
  if (n > m) return (FALSE);
  v = (n^2)*m;
  maxV = v * log(v);
  numbers = getPrimes(maxV);
  mod = sample(numbers, 1);
  
  r = number(a, 1, n, mod);
  
  for(i in 1:m-n+1)
  {
    act = number(b, i, n, mod);
    if (act == r)
        if (substr(b, i, i+n-1) == a)
          return (i);
  }
  return (-1);
}
x = "011";
y = "000011"; # x se gaseste la pozitia 4
isSubstring(x,y)
y = "000"; # x nu se gaseste
isSubstring(x,y)
y = "011"; # x se gaseste la pozitia 1
isSubstring(x,y)

# B3
# u si U sunt stringuri
u1 = "0110";
u2 = "110";
u3 = "00001";
u4 = "1111100";
u5 = "0100101";
u6 = "100101";
um = vector(mode = "list");
um[[1]] = u1;
um[[2]] = u2;
um[[3]] = u3;
um[[4]] = u5;
um

c2v1 = function(mod, r, ln, um)
{
  rv = vector();
  nr = length(um);
  lens = vector();
  
  for (i in 1:nr)
  {
    lens[i] = nchar(um[[i]]);
    rv[i] = number(um[[i]],1,lens[i],mod);
  }
  for (i in 1:nr)
  {
    if (rv[i] == r & ln == lens[i])
      return (TRUE);
  }
  return (FALSE);
}

c1v1 = function(n, u, um)
{
  n2 = n^2;
  primeNumbers = getPrimes(n2);
  mod = sample(primeNumbers, 1);
  
  ln = nchar(u);
  r = number(u, 1, ln, mod);
  
  return (c2v1(mod, r, ln, um));
}
c1v1(104, u4, um);
c1v1(100, u5, um);
c1v1(200, u6, um)

# B3
# u si U sunt vectori
u1 = c(0,1,1,0);
u2 = c(1,1,0);
u3 = c(0,0,0,0,0,1);
u4 = c(1,1,1,1,1,0,0);
u5 = c(0,1,0,0,1,0,1);
u6 = c(1,0,0,1,0,1);
um = vector(mode = "list");
um[[1]] = u1;
um[[2]] = u2;
um[[3]] = u3;
um[[4]] = u5;
um

numberFromSeq = function(u, mod)
{
  ln = length(u);
  p3 = 1;
  ans = 0;
  for (i in ln : 1)
  {
    ans = (ans + (u[i]+1)*p3) %% mod;
    p3 = (p3 * 3) %% mod;
  }
  return (ans);
}

c2v2 = function(mod, r, um)
{
  rv = vector();
  nr = length(um);
  for (i in 1:nr)
    rv[i] = numberFromSeq(um[[i]], mod);
  for (i in 1:nr)
    if (rv[i] == r)
      return (TRUE);
  return (FALSE);
}

c1v2 = function(n, u, um)
{
  n2 = n^2;
  primeNumbers = getPrimes(n2);
  mod = sample(primeNumbers, 1);
  
  r = numberFromSeq(u, mod);
  
  return (c2v2(mod, r, um));
}
c1v2(104, u4, um);
c1v2(100, u5, um);
c1v2(200, u6, um)