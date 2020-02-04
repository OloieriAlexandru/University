-- * Comenzi DML - actualizarea inregistrarilor 
-- ex1
update studenti s1 
	set bursa = 100 * 
		(select avg(valoare) 
			from studenti s2 
			join note n2 on s2.nr_matricol = n2.nr_matricol 
			group by s2.nr_matricol 
			having s2.nr_matricol = s1.nr_matricol);
-- ex2
update studenti s1 
	set bursa = 500 where 
		exists (select 1 
			from studenti s2 
			join note n2 on s2.nr_matricol = n2.nr_matricol 
			where s2.nr_matricol = s1.nr_matricol and valoare = 10);

-- * Comenzi DML - stergerea inregistrarilor
-- ex1
delete from studenti s 
	where 0 = 
		(select count(*) 
		from note n 
		where n.nr_matricol = s.nr_matricol);

-- * Comenzi DDL - creare/modificare structuri de date
-- ex1
create table medii as 
	select nume, prenume, bursa, avg(valoare) medie 
	from studenti 
	natural join note 
	group by nr_matricol, nume, prenume, bursa;
	