-- ex1
select nume||' '||prenume as student, bursa from studenti where bursa = (select max(bursa) from studenti);
-- ex2
select nume, prenume, an, grupa from studenti where (grupa, an) in (select grupa, an from studenti where lower(nume) = 'arhire') and lower(nume) != 'arhire';
-- ex3
select distinct nume, prenume, an, grupa, min(valoare) 
	from studenti 
	natural join note 
	group by an, grupa, nume, valoare, prenume 
	having (an, grupa, valoare) in 
	(select an, grupa, min(valoare) from studenti natural join note group by an, grupa);
-- ex4
select nume, prenume, grupa, avg(valoare) 
	from studenti 
	natural join note 
	group by nr_matricol, nume, prenume, grupa 
	having avg(valoare) > (select avg(valoare) from note);
-- ex5
select * from 
	(select nume, prenume, an, grupa, avg(valoare) 
		from studenti 
		natural join note 
		group by nr_matricol, nume, prenume, an, grupa order by 5 desc) 
	where rownum < 4;
-- ex6
select nume, prenume, avg(valoare) media 
	from studenti 
	natural join note 
	group by nr_matricol, nume, prenume 
	having avg(valoare) = 
	(select max(avg(valoare)) from studenti natural join note group by nr_matricol);
-- ex7
select s.nume, s.prenume, n.valoare, c.titlu_curs 
	from studenti s
	join note n on s.nr_matricol = n.nr_matricol
	join cursuri c on n.id_curs = c.id_curs 
	where lower(titlu_curs) = 'logica' 
	and valoare in (select n.valoare from studenti 
		natural join note n 
		join cursuri c on n.id_curs = c.id_curs 
		where lower(c.titlu_curs) = 'logica' and lower(nume) = 'ciobotariu' and lower(prenume) = 'ciprian') 
	and s.nr_matricol not in 
	(select nr_matricol from studenti where lower(nume) = 'ciobotariu' and lower(prenume) = 'ciprian');
-- ex8
select * from (select prenume from studenti order by 1 asc) where rownum <= 5 
	minus 
	select * from (select prenume from studenti order by 1 asc) where rownum <= 4;
-- ex8v2
select prenume from
	(select prenume, rownum rnum from
		(select prenume from studenti order by prenume))
	where rnum = 5;
-- ex9
select * from 
	(select s.nr_matricol, s.nume, s.prenume, s.bursa, s.an, s.grupa, sum(c.credite * n.valoare) puncte
	from studenti s 
	join note n on s.nr_matricol = n.nr_matricol 
	join cursuri c on c.id_curs = n.id_curs 
	group by s.nr_matricol, s.nume, s.prenume, s.bursa, s.an, s.grupa
	order by 7 desc)
	where rownum <= 3
 minus
   select * from 
	(select s.nr_matricol, s.nume, s.prenume, s.bursa, s.an, s.grupa, sum(c.credite * n.valoare) puncte
	from studenti s 
	join note n on s.nr_matricol = n.nr_matricol 
	join cursuri c on c.id_curs = n.id_curs 
	group by s.nr_matricol, s.nume, s.prenume, s.bursa, s.an, s.grupa
	order by 7 desc)
	where rownum <= 2;     
-- ex10
select nume, prenume, max(valoare), titlu_curs 
	from studenti s 
	join note n on s.nr_matricol = n.nr_matricol 
	join cursuri c on n.id_curs = c.id_curs
	group by s.nr_matricol, s.nume, s.prenume, n.id_curs, c.titlu_curs
	having (n.id_curs,max(n.valoare)) in 
	(select c2.id_curs, max(n2.valoare) 
		from cursuri c2 
		join note n2 on c2.id_curs = n2.id_curs
		group by c2.id_curs);
		