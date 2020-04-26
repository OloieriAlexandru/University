
create or replace package math_package as

    function cmmdc(p_a IN Integer, p_b IN Integer) return Integer;
    function cmmmc(p_a IN Integer, p_b IN Integer) return Integer;
    procedure cmmdc_cmmmc(p_a IN Integer, p_b IN Integer, p_cmmdc OUT Integer, p_cmmmc OUT Integer);

end math_package;

create or replace package body math_package as

    function cmmdc(p_a IN Integer, p_b IN Integer)
    return Integer as
        v_r Integer;
        v_a Integer;
        v_b Integer;
    begin
        v_a := p_a;
        v_b := p_b;
        while (v_b > 0) loop
            v_r := mod(v_a, v_b);
            v_a := v_b;
            v_b := v_r;
        end loop;
        return v_a;
    end;
    
    function cmmmc(p_a IN Integer, p_b IN Integer)
    return Integer as
        v_prod Integer;
        v_cmmdc Integer;
    begin
        v_prod := p_a * p_b;
        v_cmmdc := cmmdc(p_a, p_b);
        return v_prod / v_cmmdc;
    end;
    
    procedure cmmdc_cmmmc(p_a IN Integer, p_b IN Integer, p_cmmdc OUT Integer, p_cmmmc OUT Integer) as
    begin
        p_cmmdc := cmmdc(p_a, p_b);
        p_cmmmc := cmmmc(p_a, p_b);
    end;
    
end math_package;

declare
    v_a Integer := 30;
    v_b Integer := 45;
    v_cmmdc Integer;
    v_cmmmc Integer;
begin
    math_package.cmmdc_cmmmc(v_a, v_b, v_cmmdc, v_cmmmc);
    DBMS_Output.put_line(v_cmmdc||' '||v_cmmmc);
end;

select math_package.cmmdc(10, 20) from dual;
select math_package.cmmmc(84, 165) from dual;

select math_package.cmmdc((select sum(nvl(bursa, 0)) from studenti where an = 2 and grupa = 'A1'), 
            (select sum(nvl(bursa, 0)) from studenti where an = 1 and grupa = 'A2') ) from dual;

select sum(nvl(bursa, 0)) from studenti where an = 2 and grupa = 'A1';
select sum(nvl(bursa, 0)) from studenti where an = 1 and grupa = 'A2';