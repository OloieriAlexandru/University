# Tema 2 PS Oloieri Alexandru

# A1
B = function(n,p,lmb,k)
{
  s = seq(0,k,1);
  binomiala = dbinom(s,n,p);
  barplot(binomiala, space = 0, main="Distributia binomiala",xlab = "x", ylab = "y");
  
  geometrica = dgeom(s,p,log=FALSE);
  barplot(geometrica, space = 0, main="Distributia geometrica",xlab = "x", ylab = "y");
  
  poission = dpois(s,p,log=FALSE);
  barplot(poission, space = 0, main="Distributia poisson",xlab = "x", ylab = "y");
}
B(100,0.3,0.25,50);

# A3
sample = c(9.50,5.50,6.60,7.25,8.50,9.70,7.50,8.25,8.50,8.66,7.50,9.00,8.50,9.33,
           8.33,9.90,8.75,5.60,6.50,6.75,8.20,8.33,9.50,8.66,6.50,7.25,9.50,9.33,
           7.50,8.60,5.60,7.25,8.50,9.95,6.66,6.40,7.75,7.66,6.60,9.33,7.80,9.85,
           6.66,8.66,5.75,8.25,8.33,9.75,8.25,6.33,7.50,8.25,8.66,8.33,5.75,9.33,
           8.75,7.25,6.60,9.50,7.50,6.85,6.75,5.75,5.66,6.75,7.60,7.33,6.85,5.66);
printInfo = function(x)
{
    m = mean(x); 
    s = sd(x);
    aberante = vector();
    
    cat("Media: ",m,"\n");
    cat("Mediana: ",median(x),"\n");
    cat("Deviatia standard: ",s,"\n");
    cat("Quartilele: ",quantile(x)[2],quantile(x)[3],quantile(x)[4],"\n");
    
    j = 0;
    for (i in 1:length(x)){
      if ((x[i] < m - 2*s) | (x[i] > m + 2*s)) {
        j = j + 1;
        aberante[j] = x[i];
      }
    }
    
    if (length(aberante)>0) {
      cat("Valorile aberante: ",aberante,"\n");
    } else {
      cat("Nu exista valori aberante\n");
    }
    
    interval = seq(4,10,1);
    hist(x,breaks = interval, right = T, freq = T);
    
    summary(x);
}
printInfo(sample)