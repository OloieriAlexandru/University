# I.1
z_test = function(n, population_mean, sample_mean, alpha, disp, type)
# type == 1 -> ipoteza asim la stg
# type == 2 -> ipoteza asim la drp
# type == 3 -> ipoteza simetrica
{
  ok = FALSE;
  sigma = sqrt(disp);
  z_score = (sample_mean - population_mean)/(sigma/sqrt(n));
  if (type == 1)
  {
    z_star = qnorm(alpha, 0, 1);
    if (z_score < z_star)
      ok = TRUE;
  }
  else
  if (type == 2)
  {
    z_star = qnorm(1-alpha, 0, 1);
    if (z_score > z_star)
      ok = TRUE;
  }
  else
  {
    z_star = -qnorm(alpha/2.0,0,1);
    if (abs(z_score) > abs(z_star))
      ok = TRUE;
  }
  if (ok == TRUE)
    cat("Se accepta ipoteza alternativa, se respinge ipoteza nula\n");
  if (ok == FALSE)
    cat("Se accepta ipoteza nula, se respinge ipoteza alternativa (nu exista sufieciente dovezi)\n");
}

# Exemplu
z_test(200,810,816,0.05,50^2,2);

# I.2
z_test(49,90,88,0.05,144,1);

# I.3
z_test(36,75,85,0.01,17,3);

# I.4
z_test(100,21,20.5,0.01,2.5^2,1);

# I.5
z_test(100,1000,970,0.05,85^85,1);
z_test(100,1000,970,0.01,85^85,1);

# I.6
z_test(16,22,20,0.05,3^2,3);

# II.1

t_test = function(n, population_mean, sample_mean, alpha, s, type)
{
  t_scor = (sample_mean - population_mean) / (s/sqrt(n));
  ok = FALSE;
  if (type == 1)
  {
    t_critic = qt(alpha, n-1);
    if (t_scor < t_critic)
      ok = TRUE;
  }
  else
  if (type == 2)
  {
    t_critic = qt(1 - alpha, n-1);
    if (t_scor > t_critic)
      ok = TRUE;
  }
  else
  {
    t_critic = -qt(alpha/2, n-1);
    if (abs(t_scor) > abs(t_critic))
      ok = TRUE;
  }
  if (ok == TRUE)
    cat("Se accepta ipoteza alternativa, se respinge ipoteza nula\n");
  if (ok == FALSE)
    cat("Nu exista suficiente dovezi pentru a accepta ipoteza alternativa, se accepta ipoteza nula\n");
  
}

t_test_file = function(alpha, population_mean, type, fileName)
{
  x = scan(fileName);
  sample_mean = mean(x);
  s = sd(x);
  n = length(x);
  t_test(n,population_mean,sample_mean,alpha,s,type);
}

# II.2
x = c(36, 32, 28, 33, 41, 28, 31, 26, 29, 34);
t_test(length(x), 34, mean(x), 0.01 ,sd(x), 3);

# II.3
t_test(100,11.4, 11.9, 0.01, 0.25, 2);
t_test(100,11.4, 11.9, 0.05, 0.25, 2);

# II.4
t_test_file(0.01, 80, 3, "history.txt");
t_test_file(0.05, 80, 3, "history.txt");

# III.1
z_test_means = function(n1, sample_mean1, sd1, n2, sample_mean2, sd2, alpha, type, m0)
{
  ok = FALSE;
  z_score = ((sample_mean1 - sample_mean2) - m0) / (sqrt((sd1^2/n1)+(sd2^2/n2)));
  if (type == 1)
  {
    z_star = qnorm(alpha, 0, 1);
    if (z_score < z_star)
      ok = TRUE;
  }
  else
  if (type == 2)
  {
    z_star = qnorm(1-alpha, 0, 1);
    if (z_score > z_star)
      ok = TRUE;
  }
  else
  if (type == 3)
  {
    z_star = qnorm(1-alpha/2,0,1);
    if (abs(z_score) > abs(z_star))
      ok = TRUE;
  }
  if (ok == TRUE)
    cat("Respingem ipoteza nula, acceptam ipoteza alternativa\n");
  if (ok == FALSE)
    cat("Nu exista dovezi pentru a accepta ipoteza alternativa, acceptam ipoteza nula\n");
  ans = c(z_star, z_score);
  return (ans);
}

# III.2
z_test_means(80,160,3.24,70,155,2.25,0.01,3,0);

# III.3
z_test_means(100,22.8,1.3,100,23.3,1.9,0.01,3,0);

# IV.1
f_test = function(n1,s1,n2,s2,alpha,type)
{
  ok = FALSE;
  f_scor = (s1^2)/(s2^2);
  if (type == 1)
  {
    f_star = qf(alpha/2, n1-1,n2-1);
    if (f_scor< f_star)
      ok = TRUE;
  }
  else
  if (type == 2)
  {
    f_star = qf(1-alpha/2, n1-1,n2-1);
    if (f_scor > f_star)
      ok = TRUE;
  }
  else
  {
    f_star_left = qf(alpha/2,n1-1,n2-1);
    f_star_right = qf(1-alpha/2,n1-1,n2-1);
    if (f_scor < f_star_left | f_scor > f_star_right)
      ok = TRUE;
    cat(f_star_left," ",f_star_right,"\n");
  }
  if (ok == TRUE)
    cat("Se accepta ipoteza alternativa, se respinge ipoteza nula\n");
  if (ok == FALSE)
    cat("Se accepta ipoteza nula, se respinge ipoteza alternativa (nu exista sufieciente dovezi)\n");
}
f_test(120,5.05,135,5.44,0.01,3);

f_test_file = function(alpha, type, fileName, c1, c2)
{
  x = read.table(fileName,header = TRUE)[[c1]];
  y = read.table(fileName,header = TRUE)[[c2]];
  n1 = length(x);
  s1 = sd(x);
  n2 = length(y);
  s2 = sd(y);
  f_test(n1,s1,n2,s2,alpha,type);
}

f_test_vector = function(alpha, type, v1, v2)
{
  n1 = length(v1);
  n2 = length(v2);
  s1 = sd(v1);
  s2 = sd(v2);
  f_test(n1,s1,n2,s2,alpha,type);
}

# IV.2
f_test_file(0.01,3,"program.txt",'A','B')
f_test_file(0.05,3,"program.txt",'A','B')

# IV.3
v1 = c(12.512,12.869,19.098,15.350,13.297,15.589);
v2 = c(11.074,9.686,12.164,8.351,12.182,11.489);
f_test_vector(0.05,3,v1,v2);