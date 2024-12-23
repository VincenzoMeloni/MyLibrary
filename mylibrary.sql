PGDMP                      |         	   MyLibrary     17.1 (Ubuntu 17.1-1.pgdg24.04+1)     17.1 (Ubuntu 17.1-1.pgdg24.04+1) #    |           0    0    ENCODING    ENCODING        SET client_encoding = 'UTF8';
                           false            }           0    0 
   STDSTRINGS 
   STDSTRINGS     (   SET standard_conforming_strings = 'on';
                           false            ~           0    0 
   SEARCHPATH 
   SEARCHPATH     8   SELECT pg_catalog.set_config('search_path', '', false);
                           false                       1262    16388 	   MyLibrary    DATABASE     w   CREATE DATABASE "MyLibrary" WITH TEMPLATE = template0 ENCODING = 'UTF8' LOCALE_PROVIDER = libc LOCALE = 'it_IT.UTF-8';
    DROP DATABASE "MyLibrary";
                     postgres    false            �            1255    16445    checkout_trigger_function()    FUNCTION     x  CREATE FUNCTION public.checkout_trigger_function() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN

    FOR i IN 1..NEW.quantita LOOP
        -- Insert a row into the prestito table for each copy
        INSERT INTO prestito (id_utente, id_libro, data_prestito, data_scadenza, restituito)
        VALUES (NEW.user_id, NEW.libro_id, NOW(), NOW() + INTERVAL '14 days', FALSE);
    END LOOP;

    UPDATE libro
    SET num_copie_disponibili = num_copie_disponibili - NEW.quantita
    WHERE id_libro = NEW.libro_id AND num_copie_disponibili >= NEW.quantita;

    -- Update the max prestiti (books the user can borrow) in the utente table
    UPDATE utente
    SET max_prestiti = max_prestiti - NEW.quantita
    WHERE id_utente = NEW.user_id AND max_prestiti >= NEW.quantita;

    DELETE FROM carrello
    WHERE user_id = NEW.user_id AND libro_id = NEW.libro_id;

    RETURN NEW;
END;
$$;
 2   DROP FUNCTION public.checkout_trigger_function();
       public               postgres    false            �            1259    16389    carrello    TABLE     �   CREATE TABLE public.carrello (
    user_id integer NOT NULL,
    libro_id integer NOT NULL,
    is_checkout boolean DEFAULT false,
    quantita integer DEFAULT 1
);
    DROP TABLE public.carrello;
       public         heap r       postgres    false            �            1259    16392    libro    TABLE     �  CREATE TABLE public.libro (
    id_libro integer NOT NULL,
    titolo character varying(200) NOT NULL,
    autore character varying(200) NOT NULL,
    num_copie_tot integer NOT NULL,
    num_copie_disponibili integer NOT NULL,
    genere character varying(255),
    CONSTRAINT libro_num_copie_disponibili_check CHECK ((num_copie_disponibili >= 0)),
    CONSTRAINT libro_num_copie_tot_check CHECK ((num_copie_tot >= 0))
);
    DROP TABLE public.libro;
       public         heap r       postgres    false            �            1259    16399    libro_id_libro_seq    SEQUENCE     �   CREATE SEQUENCE public.libro_id_libro_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 )   DROP SEQUENCE public.libro_id_libro_seq;
       public               postgres    false    218            �           0    0    libro_id_libro_seq    SEQUENCE OWNED BY     I   ALTER SEQUENCE public.libro_id_libro_seq OWNED BY public.libro.id_libro;
          public               postgres    false    219            �            1259    16400    prestito    TABLE        CREATE TABLE public.prestito (
    id_prestito integer NOT NULL,
    id_libro integer NOT NULL,
    id_utente integer NOT NULL,
    data_prestito timestamp without time zone DEFAULT CURRENT_TIMESTAMP,
    data_scadenza timestamp without time zone,
    restituito boolean DEFAULT false
);
    DROP TABLE public.prestito;
       public         heap r       postgres    false            �            1259    16405    prestito_id_prestito_seq    SEQUENCE     �   CREATE SEQUENCE public.prestito_id_prestito_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 /   DROP SEQUENCE public.prestito_id_prestito_seq;
       public               postgres    false    220            �           0    0    prestito_id_prestito_seq    SEQUENCE OWNED BY     U   ALTER SEQUENCE public.prestito_id_prestito_seq OWNED BY public.prestito.id_prestito;
          public               postgres    false    221            �            1259    16406    utente    TABLE     �   CREATE TABLE public.utente (
    id_utente integer NOT NULL,
    username character varying(100) NOT NULL,
    password character varying(255) NOT NULL,
    max_prestiti integer DEFAULT 5
);
    DROP TABLE public.utente;
       public         heap r       postgres    false            �            1259    16409    utente_id_utente_seq    SEQUENCE     �   CREATE SEQUENCE public.utente_id_utente_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 +   DROP SEQUENCE public.utente_id_utente_seq;
       public               postgres    false    222            �           0    0    utente_id_utente_seq    SEQUENCE OWNED BY     M   ALTER SEQUENCE public.utente_id_utente_seq OWNED BY public.utente.id_utente;
          public               postgres    false    223            �           2604    16410    libro id_libro    DEFAULT     p   ALTER TABLE ONLY public.libro ALTER COLUMN id_libro SET DEFAULT nextval('public.libro_id_libro_seq'::regclass);
 =   ALTER TABLE public.libro ALTER COLUMN id_libro DROP DEFAULT;
       public               postgres    false    219    218            �           2604    16411    prestito id_prestito    DEFAULT     |   ALTER TABLE ONLY public.prestito ALTER COLUMN id_prestito SET DEFAULT nextval('public.prestito_id_prestito_seq'::regclass);
 C   ALTER TABLE public.prestito ALTER COLUMN id_prestito DROP DEFAULT;
       public               postgres    false    221    220            �           2604    16412    utente id_utente    DEFAULT     t   ALTER TABLE ONLY public.utente ALTER COLUMN id_utente SET DEFAULT nextval('public.utente_id_utente_seq'::regclass);
 ?   ALTER TABLE public.utente ALTER COLUMN id_utente DROP DEFAULT;
       public               postgres    false    223    222            s          0    16389    carrello 
   TABLE DATA           L   COPY public.carrello (user_id, libro_id, is_checkout, quantita) FROM stdin;
    public               postgres    false    217            t          0    16392    libro 
   TABLE DATA           g   COPY public.libro (id_libro, titolo, autore, num_copie_tot, num_copie_disponibili, genere) FROM stdin;
    public               postgres    false    218            v          0    16400    prestito 
   TABLE DATA           n   COPY public.prestito (id_prestito, id_libro, id_utente, data_prestito, data_scadenza, restituito) FROM stdin;
    public               postgres    false    220            x          0    16406    utente 
   TABLE DATA           M   COPY public.utente (id_utente, username, password, max_prestiti) FROM stdin;
    public               postgres    false    222            �           0    0    libro_id_libro_seq    SEQUENCE SET     @   SELECT pg_catalog.setval('public.libro_id_libro_seq', 5, true);
          public               postgres    false    219            �           0    0    prestito_id_prestito_seq    SEQUENCE SET     G   SELECT pg_catalog.setval('public.prestito_id_prestito_seq', 35, true);
          public               postgres    false    221            �           0    0    utente_id_utente_seq    SEQUENCE SET     C   SELECT pg_catalog.setval('public.utente_id_utente_seq', 15, true);
          public               postgres    false    223            �           2606    16414    carrello carrello_pkey 
   CONSTRAINT     c   ALTER TABLE ONLY public.carrello
    ADD CONSTRAINT carrello_pkey PRIMARY KEY (user_id, libro_id);
 @   ALTER TABLE ONLY public.carrello DROP CONSTRAINT carrello_pkey;
       public                 postgres    false    217    217            �           2606    16416    libro libro_pkey 
   CONSTRAINT     T   ALTER TABLE ONLY public.libro
    ADD CONSTRAINT libro_pkey PRIMARY KEY (id_libro);
 :   ALTER TABLE ONLY public.libro DROP CONSTRAINT libro_pkey;
       public                 postgres    false    218            �           2606    16418    prestito prestito_pkey 
   CONSTRAINT     ]   ALTER TABLE ONLY public.prestito
    ADD CONSTRAINT prestito_pkey PRIMARY KEY (id_prestito);
 @   ALTER TABLE ONLY public.prestito DROP CONSTRAINT prestito_pkey;
       public                 postgres    false    220            �           2606    16420    utente utente_pkey 
   CONSTRAINT     W   ALTER TABLE ONLY public.utente
    ADD CONSTRAINT utente_pkey PRIMARY KEY (id_utente);
 <   ALTER TABLE ONLY public.utente DROP CONSTRAINT utente_pkey;
       public                 postgres    false    222            �           2606    16422    utente utente_username_key 
   CONSTRAINT     Y   ALTER TABLE ONLY public.utente
    ADD CONSTRAINT utente_username_key UNIQUE (username);
 D   ALTER TABLE ONLY public.utente DROP CONSTRAINT utente_username_key;
       public                 postgres    false    222            �           2620    16447    carrello checkout_trigger    TRIGGER     �   CREATE TRIGGER checkout_trigger AFTER UPDATE OF is_checkout ON public.carrello FOR EACH ROW WHEN ((new.is_checkout = true)) EXECUTE FUNCTION public.checkout_trigger_function();
 2   DROP TRIGGER checkout_trigger ON public.carrello;
       public               postgres    false    217    224    217    217            �           2606    16423    carrello carrello_libro_id_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.carrello
    ADD CONSTRAINT carrello_libro_id_fkey FOREIGN KEY (libro_id) REFERENCES public.libro(id_libro) ON DELETE CASCADE;
 I   ALTER TABLE ONLY public.carrello DROP CONSTRAINT carrello_libro_id_fkey;
       public               postgres    false    218    3286    217            �           2606    16428    carrello carrello_user_id_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.carrello
    ADD CONSTRAINT carrello_user_id_fkey FOREIGN KEY (user_id) REFERENCES public.utente(id_utente) ON DELETE CASCADE;
 H   ALTER TABLE ONLY public.carrello DROP CONSTRAINT carrello_user_id_fkey;
       public               postgres    false    3290    222    217            �           2606    16433    prestito prestito_id_libro_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.prestito
    ADD CONSTRAINT prestito_id_libro_fkey FOREIGN KEY (id_libro) REFERENCES public.libro(id_libro) ON DELETE CASCADE;
 I   ALTER TABLE ONLY public.prestito DROP CONSTRAINT prestito_id_libro_fkey;
       public               postgres    false    218    220    3286            �           2606    16438     prestito prestito_id_utente_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.prestito
    ADD CONSTRAINT prestito_id_utente_fkey FOREIGN KEY (id_utente) REFERENCES public.utente(id_utente) ON DELETE CASCADE;
 J   ALTER TABLE ONLY public.prestito DROP CONSTRAINT prestito_id_utente_fkey;
       public               postgres    false    222    220    3290            s   +   x�3�4�L�4�2�4��&`��0��4�	�b���� �H      t   �   x�-��n�@D�ޯ�
�W�6�J­7X�bcK������͛Y×6D1d�vd��C����)eVx�546�>,l��d#�c$l,��_�l������B���V5��;--��)���M�@��Ԥ��=��Ǥ���p�>�a����V���Z~>�U�Y�@w-G��]l������~!�?(BKS      v      x���ɍ�0@ѳ\��]k�9��0tf0ђ�߾$S2y�B���A�@>��`6����|K���~=j3i��)%o�J�p9C%DA�m\�n)y�VǏ��K�К�|���`��7o��dM>�Ԕ_N4i�|M���'�����B<z� 	V0�>�9��"��|:�����SJ�7���~�N&_�-L��U�J�CbQ{Ϯ��������5��ۙ�+,��!�߷���y�W��O)=�ނk�Aw���_Rz���Vw���Rz��M�z���>��/)���O8����B      x   �   x�%�K�0Dד� ҏwac���Hl�TE��8ec��f�P�E�f,�9Lh\v&GE����;I�F$���q��E�,K�C�w,�Z�[{I��r��g6�t��d��ܶx���W���eU$!N^��㌕՟�6���K�=u      #    |           0    0    ENCODING    ENCODING        SET client_encoding = 'UTF8';
                           false            }           0    0 
   STDSTRINGS 
   STDSTRINGS     (   SET standard_conforming_strings = 'on';
                           false            ~           0    0 
   SEARCHPATH 
   SEARCHPATH     8   SELECT pg_catalog.set_config('search_path', '', false);
                           false                       1262    16388 	   MyLibrary    DATABASE     w   CREATE DATABASE "MyLibrary" WITH TEMPLATE = template0 ENCODING = 'UTF8' LOCALE_PROVIDER = libc LOCALE = 'it_IT.UTF-8';
    DROP DATABASE "MyLibrary";
                     postgres    false            �            1255    16445    checkout_trigger_function()    FUNCTION     x  CREATE FUNCTION public.checkout_trigger_function() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN

    FOR i IN 1..NEW.quantita LOOP
        -- Insert a row into the prestito table for each copy
        INSERT INTO prestito (id_utente, id_libro, data_prestito, data_scadenza, restituito)
        VALUES (NEW.user_id, NEW.libro_id, NOW(), NOW() + INTERVAL '14 days', FALSE);
    END LOOP;

    UPDATE libro
    SET num_copie_disponibili = num_copie_disponibili - NEW.quantita
    WHERE id_libro = NEW.libro_id AND num_copie_disponibili >= NEW.quantita;

    -- Update the max prestiti (books the user can borrow) in the utente table
    UPDATE utente
    SET max_prestiti = max_prestiti - NEW.quantita
    WHERE id_utente = NEW.user_id AND max_prestiti >= NEW.quantita;

    DELETE FROM carrello
    WHERE user_id = NEW.user_id AND libro_id = NEW.libro_id;

    RETURN NEW;
END;
$$;
 2   DROP FUNCTION public.checkout_trigger_function();
       public               postgres    false            �            1259    16389    carrello    TABLE     �   CREATE TABLE public.carrello (
    user_id integer NOT NULL,
    libro_id integer NOT NULL,
    is_checkout boolean DEFAULT false,
    quantita integer DEFAULT 1
);
    DROP TABLE public.carrello;
       public         heap r       postgres    false            �            1259    16392    libro    TABLE     �  CREATE TABLE public.libro (
    id_libro integer NOT NULL,
    titolo character varying(200) NOT NULL,
    autore character varying(200) NOT NULL,
    num_copie_tot integer NOT NULL,
    num_copie_disponibili integer NOT NULL,
    genere character varying(255),
    CONSTRAINT libro_num_copie_disponibili_check CHECK ((num_copie_disponibili >= 0)),
    CONSTRAINT libro_num_copie_tot_check CHECK ((num_copie_tot >= 0))
);
    DROP TABLE public.libro;
       public         heap r       postgres    false            �            1259    16399    libro_id_libro_seq    SEQUENCE     �   CREATE SEQUENCE public.libro_id_libro_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 )   DROP SEQUENCE public.libro_id_libro_seq;
       public               postgres    false    218            �           0    0    libro_id_libro_seq    SEQUENCE OWNED BY     I   ALTER SEQUENCE public.libro_id_libro_seq OWNED BY public.libro.id_libro;
          public               postgres    false    219            �            1259    16400    prestito    TABLE        CREATE TABLE public.prestito (
    id_prestito integer NOT NULL,
    id_libro integer NOT NULL,
    id_utente integer NOT NULL,
    data_prestito timestamp without time zone DEFAULT CURRENT_TIMESTAMP,
    data_scadenza timestamp without time zone,
    restituito boolean DEFAULT false
);
    DROP TABLE public.prestito;
       public         heap r       postgres    false            �            1259    16405    prestito_id_prestito_seq    SEQUENCE     �   CREATE SEQUENCE public.prestito_id_prestito_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 /   DROP SEQUENCE public.prestito_id_prestito_seq;
       public               postgres    false    220            �           0    0    prestito_id_prestito_seq    SEQUENCE OWNED BY     U   ALTER SEQUENCE public.prestito_id_prestito_seq OWNED BY public.prestito.id_prestito;
          public               postgres    false    221            �            1259    16406    utente    TABLE     �   CREATE TABLE public.utente (
    id_utente integer NOT NULL,
    username character varying(100) NOT NULL,
    password character varying(255) NOT NULL,
    max_prestiti integer DEFAULT 5
);
    DROP TABLE public.utente;
       public         heap r       postgres    false            �            1259    16409    utente_id_utente_seq    SEQUENCE     �   CREATE SEQUENCE public.utente_id_utente_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 +   DROP SEQUENCE public.utente_id_utente_seq;
       public               postgres    false    222            �           0    0    utente_id_utente_seq    SEQUENCE OWNED BY     M   ALTER SEQUENCE public.utente_id_utente_seq OWNED BY public.utente.id_utente;
          public               postgres    false    223            �           2604    16410    libro id_libro    DEFAULT     p   ALTER TABLE ONLY public.libro ALTER COLUMN id_libro SET DEFAULT nextval('public.libro_id_libro_seq'::regclass);
 =   ALTER TABLE public.libro ALTER COLUMN id_libro DROP DEFAULT;
       public               postgres    false    219    218            �           2604    16411    prestito id_prestito    DEFAULT     |   ALTER TABLE ONLY public.prestito ALTER COLUMN id_prestito SET DEFAULT nextval('public.prestito_id_prestito_seq'::regclass);
 C   ALTER TABLE public.prestito ALTER COLUMN id_prestito DROP DEFAULT;
       public               postgres    false    221    220            �           2604    16412    utente id_utente    DEFAULT     t   ALTER TABLE ONLY public.utente ALTER COLUMN id_utente SET DEFAULT nextval('public.utente_id_utente_seq'::regclass);
 ?   ALTER TABLE public.utente ALTER COLUMN id_utente DROP DEFAULT;
       public               postgres    false    223    222            s          0    16389    carrello 
   TABLE DATA           L   COPY public.carrello (user_id, libro_id, is_checkout, quantita) FROM stdin;
    public               postgres    false    217   �-       t          0    16392    libro 
   TABLE DATA           g   COPY public.libro (id_libro, titolo, autore, num_copie_tot, num_copie_disponibili, genere) FROM stdin;
    public               postgres    false    218   -.       v          0    16400    prestito 
   TABLE DATA           n   COPY public.prestito (id_prestito, id_libro, id_utente, data_prestito, data_scadenza, restituito) FROM stdin;
    public               postgres    false    220   �.       x          0    16406    utente 
   TABLE DATA           M   COPY public.utente (id_utente, username, password, max_prestiti) FROM stdin;
    public               postgres    false    222   /0       �           0    0    libro_id_libro_seq    SEQUENCE SET     @   SELECT pg_catalog.setval('public.libro_id_libro_seq', 5, true);
          public               postgres    false    219            �           0    0    prestito_id_prestito_seq    SEQUENCE SET     G   SELECT pg_catalog.setval('public.prestito_id_prestito_seq', 35, true);
          public               postgres    false    221            �           0    0    utente_id_utente_seq    SEQUENCE SET     C   SELECT pg_catalog.setval('public.utente_id_utente_seq', 15, true);
          public               postgres    false    223            �           2606    16414    carrello carrello_pkey 
   CONSTRAINT     c   ALTER TABLE ONLY public.carrello
    ADD CONSTRAINT carrello_pkey PRIMARY KEY (user_id, libro_id);
 @   ALTER TABLE ONLY public.carrello DROP CONSTRAINT carrello_pkey;
       public                 postgres    false    217    217            �           2606    16416    libro libro_pkey 
   CONSTRAINT     T   ALTER TABLE ONLY public.libro
    ADD CONSTRAINT libro_pkey PRIMARY KEY (id_libro);
 :   ALTER TABLE ONLY public.libro DROP CONSTRAINT libro_pkey;
       public                 postgres    false    218            �           2606    16418    prestito prestito_pkey 
   CONSTRAINT     ]   ALTER TABLE ONLY public.prestito
    ADD CONSTRAINT prestito_pkey PRIMARY KEY (id_prestito);
 @   ALTER TABLE ONLY public.prestito DROP CONSTRAINT prestito_pkey;
       public                 postgres    false    220            �           2606    16420    utente utente_pkey 
   CONSTRAINT     W   ALTER TABLE ONLY public.utente
    ADD CONSTRAINT utente_pkey PRIMARY KEY (id_utente);
 <   ALTER TABLE ONLY public.utente DROP CONSTRAINT utente_pkey;
       public                 postgres    false    222            �           2606    16422    utente utente_username_key 
   CONSTRAINT     Y   ALTER TABLE ONLY public.utente
    ADD CONSTRAINT utente_username_key UNIQUE (username);
 D   ALTER TABLE ONLY public.utente DROP CONSTRAINT utente_username_key;
       public                 postgres    false    222            �           2620    16447    carrello checkout_trigger    TRIGGER     �   CREATE TRIGGER checkout_trigger AFTER UPDATE OF is_checkout ON public.carrello FOR EACH ROW WHEN ((new.is_checkout = true)) EXECUTE FUNCTION public.checkout_trigger_function();
 2   DROP TRIGGER checkout_trigger ON public.carrello;
       public               postgres    false    217    224    217    217            �           2606    16423    carrello carrello_libro_id_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.carrello
    ADD CONSTRAINT carrello_libro_id_fkey FOREIGN KEY (libro_id) REFERENCES public.libro(id_libro) ON DELETE CASCADE;
 I   ALTER TABLE ONLY public.carrello DROP CONSTRAINT carrello_libro_id_fkey;
       public               postgres    false    218    3286    217            �           2606    16428    carrello carrello_user_id_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.carrello
    ADD CONSTRAINT carrello_user_id_fkey FOREIGN KEY (user_id) REFERENCES public.utente(id_utente) ON DELETE CASCADE;
 H   ALTER TABLE ONLY public.carrello DROP CONSTRAINT carrello_user_id_fkey;
       public               postgres    false    3290    222    217            �           2606    16433    prestito prestito_id_libro_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.prestito
    ADD CONSTRAINT prestito_id_libro_fkey FOREIGN KEY (id_libro) REFERENCES public.libro(id_libro) ON DELETE CASCADE;
 I   ALTER TABLE ONLY public.prestito DROP CONSTRAINT prestito_id_libro_fkey;
       public               postgres    false    218    220    3286            �           2606    16438     prestito prestito_id_utente_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.prestito
    ADD CONSTRAINT prestito_id_utente_fkey FOREIGN KEY (id_utente) REFERENCES public.utente(id_utente) ON DELETE CASCADE;
 J   ALTER TABLE ONLY public.prestito DROP CONSTRAINT prestito_id_utente_fkey;
       public               postgres    false    222    220    3290           