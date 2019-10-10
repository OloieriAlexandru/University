ex11 = function(x)
{
  v = stem(x);
}
x = scan('sample1.txt')
ex11(x)

ex12 = function(x)
{
  rate = x[['rate']];
  hist(rate, breaks=c(0,4,6,8,10,12,14,30),freq=T,right=T, col='blue');
}
x = read.csv("unemploy2012.csv", header = T, sep = ';');
ex12(x);

ex13 = function(x)
{
  a = 7;
  male = x[['male']];
  female = x[['female']];
  hist(male, breaks = a, freq=T,right=T, col='red');
  hist(female, breaks = a, freq=T, right=T, col='yellow');
}
x = read.csv("life_expect.csv", header = T, sep=',');
ex13(x);

ex21 = function(x)
{
  med=mean(x);
  medi=median(x);
  return (list(med, medi))
}
x = scan('sample1.txt');
ex21(x);

ex22 = function(x)
{
  male = x[['male']]
  female = x[['female']]
  medmale = mean(male);
  medfemale = mean(female);
  medimale = median(male);
  medifemale = median(female);
  return (list(medmale, medfemale, medimale, medifemale));
}
x = read.csv("life_expect.csv", header = T, sep=',');
ex22(x);


outliers_mean = function(x)
{
  m = mean(x);
  s = sd(x);
  new_sample = vector();
  j = 0;
  for (i in 1:length(x))
  {
    if (!(x[i] >= m - 2*s & x[i] <= m + 2*s)){
      j = j+1;
      new_sample[j] = x[i];
    }
  }
  return (new_sample);
}
sample = c(1, 91, 38, 72, 13, 27, 11, 85, 5, 22, 20, 19, 8, 17, 11, 15, 13, 23, 14, 17);
#outliers_mean(sample);

outliers_iqr = function(x)
{
  data = quantile(x);
  iqr = data[[4]] - data[[2]];
  altX = vector();
  j = 0;
  for (i in 1:length(x))
  {
    if (!(x[i]>=data[[2]] - 1.5*iqr & x[i]<=data[[4]] + 1.5*iqr)){
      j = j+1;
      altX[j] = x[i];
    }
  }
  return (altX);
}
sample = c(1, 91, 38, 72, 13, 27, 11, 85, 5, 22, 20, 19, 8, 17, 11, 15, 13, 23, 14, 17);
#sample = scan('sample2.txt')
outliers_iqr(sample);

ex33 = function(x)
{
   ans = summary(x);
   return (ans);
}
sample = scan('sample2.txt')
outliers_iqr(sample);
outliers_mean(sample);
ex33(sample);


