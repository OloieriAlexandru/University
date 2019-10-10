#Functii utile:
rep(1:5);
rep(1:2,5);
rep(1:3,2);
rep(1:4,tumes=1:4);

# alfabetul limbii engleze
x = letters[1:26];
x[1:3];

# matrice cu 2 linii 2 coloane, valori intregi de la 1 la 0 ce se pot repeta
A = matrix(sample.int(10,replace=T),nrow=2,ncol=2);
A

# exercitiul 1 rezolvat
matrixProduct = function(A, B, C) {
  n = nrow(A);
  r = matrix( , nrow = n, ncol = 1);
  x = matrix( , nrow = n, ncol = 1);
  y = matrix( , nrow = n, ncol = 1);
  r = sample(0:1, n, replace = TRUE);
  for(i in 1:n) {# x = Br
    x[i] = 0;
    for(j in 1:nrow(B))
      x[i] = (x[i]+ B[i,j]*r[j])%%2;
  }
  for(i in 1:nrow(B)) {# y = Ax = ABr
    y[i] = 0;
    for(j in 1:n)
      y[i] = (y[i]+ A[i,j]*x[j])%%2;
  }
  for(i in 1:n) {# x = Cr
    x[i] = 0;
    for(j in 1:n)
      x[i] = (x[i]+ C[i,j]*r[j])%%2;
  }
  for(i in 1:n) {# verify if ABr==Cr
    if(y[i] !=x[i])
      return(FALSE);
  }
  return(TRUE);
}
x = c(1,2,3,1);
A = matrix(x,2,2);
x = c(2,1,3,2);
B = matrix(x,2,2);
x = c(8,5,9,5);
C = matrix(x,2,2);
matrixProduct(A,B,C);

# exercitiul 2 rezolvat
treeEval = function(i, leaves)
{
  a = runif(1, 0, 1);
  len = length(leaves);
  if (log(i,2) >= log(len,2)-1)
  {
    if (a <= 0.5)
    {
      if (leaves[2*i-len+1] == 0)
      {
        return (leaves[2*i+1-len+1]);
      }
      return (1);
    }
    else
    {
      if (leaves[2*i+1-len+1]==0)
      {
        return (leaves[2*i+1-len+1]);
      }
      return (1);
    }
  }
  if ((floor(log(i,2))%%2 ==0)) # nodul este de tip MIN
  {
    if (a <= 0.5)
    {
      if (treeEval(2*i, leaves) == 1)
      {
        return (treeEval(2*i+1, leaves));
      }
      return (0);
    }
    else
    {
      if (treeEval(2*i+1, leaves) == 1)
      {
        return (treeEval(2*i, leaves));
      }
      return (0);
    }
  }
  if (a <= 0.5)
  {
     if (treeEval(2*i, leaves) == 0) 
     {
       return (treeEval(2*i+1, leaves));
     }
    return (1); # 1 or orice = 1
  }
  else
  {
    if (treeEval(2*i+1, leaves) == 0)
    {
      return (treeEval(2*i, leaves));
    }
    return (1);
  }
}
leaves = c(0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0);

gameTreeEval = function(leaves)
{
  return (treeEval(1, leaves));
}
gameTreeEval(leaves);

#Exercitii propuse
variabila_aleatoare = function(n)
{
  v = vector();
  nr = vector();
  sum = 0;
  for (i in 1:2*n)
    v[i] = 0;
  for (i in 1:n)
    v[i] = i;
  
  for (i in 1:n)
  {
    nr[i] = sample(10000,1);
    sum = sum + nr[i];
  }
  
  for (i in 1:n)
  {
    v[n+i] = ((nr[i] / sum) * 1);
  }
  
  var = matrix(v,nrow=2,ncol=n,byrow = TRUE);
  return (var);
}

variabila_aleatoare(5);