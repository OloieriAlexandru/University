CREATE OR REPLACE FUNCTION get_std_json(p_std_id INTEGER, p_std_first_name studenti.prenume%type, p_std_last_name studenti.nume%type, p_count INTEGER)
RETURN varchar2 AS
    v_json varchar2(1023) := '';
BEGIN
    v_json := '{ "id" : ' || p_std_id || ', "firstName" : "' || p_std_first_name || '", "lastName" : "' || p_std_last_name || '", "count" : ' || p_count || ' }';
    return v_json;
END;
/

CREATE OR REPLACE FUNCTION get_friends(p_id_student INTEGER)
RETURN varchar2 AS
    TYPE frequency_table IS TABLE OF INTEGER INDEX BY PLS_INTEGER;
    students_frequency frequency_table;
    
    TYPE indexes_array IS VARRAY(2000) OF PLS_INTEGER;
    sorted_indexes indexes_array;
    
    CURSOR friends_list (p_id_stud prieteni.id_student1%type) IS
        SELECT DECODE(id_student1, p_id_stud, id_student2, id_student1) FROM prieteni WHERE id_student1 = p_id_stud or id_student2 = p_id_stud;
    
    CURSOR friends_of_friends_list (p_id_stud prieteni.id_student1%type, p_id_friend prieteni.id_student1%type) IS
        SELECT DECODE(id_student1, p_id_friend, id_student2, id_student1) FROM prieteni WHERE (id_student1 = p_id_friend AND id_student2 NOT IN
            (SELECT id FROM studenti WHERE id NOT IN (SELECT DECODE(id_student1, p_id_stud, id_student2, id_student1) FROM prieteni WHERE id_student1 = p_id_stud or id_student2 = p_id_stud)) ) OR (id_student2 = p_id_friend AND id_student1 NOT IN
            (SELECT id FROM studenti WHERE id NOT IN (SELECT DECODE(id_student1, p_id_stud, id_student2, id_student1) FROM prieteni WHERE id_student1 = p_id_stud or id_student2 = p_id_stud)) );
    
    v_friend prieteni.id_student1%type;
    v_friend_of_friend prieteni.id_student1%type;
    v_json varchar2(32767) := '';
    
    v_idx INTEGER;
    v_n INTEGER;
    v_running INTEGER;
    
    v_student_id studenti.nr_matricol%type;
    v_student_first_name studenti.prenume%type;
    v_student_last_name studenti.nume%type;
    
    v_current_friend PLS_INTEGER;
    v_aux PLS_INTEGER;
BEGIN
    SELECT nr_matricol into v_student_id FROM studenti WHERE id = p_id_student;

    OPEN friends_list(p_id_student);
    LOOP
        FETCH friends_list INTO v_friend;
        EXIT WHEN friends_list%NOTFOUND;
        
        OPEN friends_of_friends_list(p_id_student, v_friend);
        LOOP
            FETCH friends_of_friends_list INTO v_friend_of_friend;
            EXIT WHEN friends_of_friends_list%NOTFOUND;
            
            IF students_frequency.EXISTS(v_friend_of_friend) THEN
                students_frequency(v_friend_of_friend) := students_frequency(v_friend_of_friend) + 1;
            ELSE
                students_frequency(v_friend_of_friend) := 1;
            END IF;
            
        END LOOP;
        CLOSE friends_of_friends_list;
        
    END LOOP;
    CLOSE friends_list;
        
    sorted_indexes := indexes_array();
    
    v_current_friend := students_frequency.FIRST;
    v_idx := 1;
    while v_current_friend IS NOT NULL LOOP
        sorted_indexes.EXTEND(1);
        sorted_indexes(v_idx) := v_current_friend;
        v_idx := v_idx + 1;
        
        v_current_friend := students_frequency.NEXT(v_current_friend);
    END LOOP;
    
    v_n := sorted_indexes.COUNT;
    v_running := 1;
    
    WHILE (v_running = 1) LOOP
        v_running := 0;
        v_idx := 1;
        WHILE (v_idx + 1 <= v_n) LOOP
            IF (students_frequency(sorted_indexes(v_idx)) < students_frequency(sorted_indexes(v_idx+1))) THEN
                v_aux := sorted_indexes(v_idx);
                sorted_indexes(v_idx) := sorted_indexes(v_idx+1);
                sorted_indexes(v_idx+1) := v_aux;
                v_running := 1;
            END IF;
            v_idx := v_idx + 1;
        END LOOP;
    END LOOP;
    
    v_json := '{ "suggested_friends" : [ ';
    
    FOR i in sorted_indexes.FIRST..sorted_indexes.LAST LOOP
        EXIT WHEN i > 5;
        
        IF i > 1 THEN
            v_json := v_json || ', ';
        END IF;
        
        SELECT prenume, nume INTO v_student_first_name, v_student_last_name FROM studenti WHERE id = sorted_indexes(i);    
        v_json := v_json || get_std_json(sorted_indexes(i), v_student_first_name, v_student_last_name, students_frequency(sorted_indexes(i)));
        
    END LOOP;
    
    v_json := v_json || ' ] }';
    
    return v_json;
EXCEPTION
    WHEN no_data_found THEN
    return '{ "error_message" : "Invalid student id!" }';
END;
/
