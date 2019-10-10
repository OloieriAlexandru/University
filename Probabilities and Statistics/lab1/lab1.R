#ex1
ex1 = function(v) {
  v=c(46,33,39,37,46,30,48,32,49,35,30,48);
  a=min(v);
  b=max(v);
  c=sum(v);
  d=length(v[v>40]);
  e=length(v[v>35])/(length(v)*100);
  return (list(a,b,c,d,e));
}
ex1()

#ex2
ex2a = function(x) {
  v=x/sum(x);
  return (v);
}
ex2a(c(1,2,3,4,5));

ex2b = function(x) {
  minn=min(x);
  maxx=max(x);
  v=(x-minn)/maxx;
  return (v);
}
ex2b(c(1,2,3,4,5));

#ex3
ex3 = function() {
  x=scan("f2.txt");
  v=x/sum(x);
  return (v);
}
ex3();

#ex4
ex4 = function(n,p) {
  return (barplot(dbinom(0:n,n,p),main='Exercitiul 4',col='green'));
}
ex4(25,0.3);

#ex5
ex5 = function(n,p) {
  return (max(dbinom(0:n,n,p)));
}
ex5(10,0.25);

#ex6
ex6 = function(n, p) {
  x=scan("f2.txt");
  v<-dgeom(x,p);
  return (barplot(v,main='Exercitiul 6', col='blue'));
}
ex6(10,0.25);

#ex7
ex7 = function(file) {
  y = read.table(file, header = T);
  a = y[['AA']];
  b = y[['BB']];
  return(plot(a,b,type='h',main='Exercitiul 7',sub='Puncte in plan',xlab='x',ylab='y', lwd=10));
}
ex7("lab1.txt");