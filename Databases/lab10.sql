1. select * from studenti s1 
	where exists 
	(select 1 from studenti s2 
	where s1.an = s2.an and s1.data_nastere < s2.data_nastere);
2. select nume, prenume, avg(n1.valoare) 
	from studenti s1 
	natural join note n1 
	group by nr_matricol, nume, prenume, s1.an 
	having avg(n1.valoare) > 
	(select avg(n2.valoare) from studenti s2 natural join note n2 group by an having s2.an = s1.an);
3. select nume, prenume, an, grupa, avg(n1.valoare) 
	from studenti s1 
	natural join note n1 
	group by nr_matricol, nume, prenume, an, grupa 
	having avg(n1.valoare) = 
	(select max(avg(n2.valoare)) from studenti s2 natural join note n2 group by nr_matricol, an, grupa having s2.an = s1.an and s2.grupa = s1.grupa);
4. select distinct s1.nume, s1.prenume, s1.grupa, s1.an
	from studenti s1
	join note n1 on s1.nr_matricol = n1.nr_matricol
	join cursuri c1 on n1.id_curs = c1.id_curs
	where exists
	(
	select 1 from studenti s2
	join note n2 on s2.nr_matricol = n2.nr_matricol
	join cursuri c2 on n2.id_curs = c2.id_curs
	group by s2.nr_matricol, n2.valoare, s2.an, c2.titlu_curs
	having n1.valoare = n2.valoare and s1.an = s2.an and c1.titlu_curs = c2.titlu_curs
	)
	group by s1.nr_matricol, s1.nume, s1.prenume, s1.grupa, s1.an, n1.valoare, c1.titlu_curs;   
5. select * from studenti s1 where 1 = (select count(*) from studenti s2 where s2.grupa = s1.grupa);
6. select p.nume, p.prenume, avg(n.valoare) 
	from note n 
	join cursuri c on n.id_curs = c.id_curs 
	join didactic d on d.id_curs = c.id_curs 
	join profesori p on d.id_prof = p.id_prof
	group by p.id_prof, p.nume, p.prenume 
	having exists ( select 1 from note n2
					join cursuri c2 on n2.id_curs = c2.id_curs
					join didactic d2 on d2.id_curs = c2.id_curs
					join profesori p2 on d2.id_prof = p2.id_prof
					group by p2.id_prof,p2.nume,p2.prenume 
					having avg(n2.valoare) = avg(n.valoare) and p2.id_prof <> p.id_prof ); 
7. select nume, (select AVG(valoare) from note where nr_matricol = s.nr_matricol) as medie from studenti s;
8v1. select * from cursuri c1 where credite = (select max(credite) from cursuri c2 where c1.an = c2.an);
8v2. select * from cursuri where (an,credite) in (select an, max(credite) from cursuri group by an);	