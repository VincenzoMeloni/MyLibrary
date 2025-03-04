PGDMP      
            
    |            libreria    17.0    17.0 !               0    0    ENCODING    ENCODING        SET client_encoding = 'UTF8';
                           false                       0    0 
   STDSTRINGS 
   STDSTRINGS     (   SET standard_conforming_strings = 'on';
                           false                       0    0 
   SEARCHPATH 
   SEARCHPATH     8   SELECT pg_catalog.set_config('search_path', '', false);
                           false                       1262    16387    libreria    DATABASE     {   CREATE DATABASE libreria WITH TEMPLATE = template0 ENCODING = 'UTF8' LOCALE_PROVIDER = libc LOCALE = 'Italian_Italy.1252';
    DROP DATABASE libreria;
                     postgres    false            �            1259    24617    carrello    TABLE     ^   CREATE TABLE public.carrello (
    user_id integer NOT NULL,
    libro_id integer NOT NULL
);
    DROP TABLE public.carrello;
       public         heap r       postgres    false            �            1259    16398    libro    TABLE     �  CREATE TABLE public.libro (
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
       public         heap r       postgres    false            �            1259    16397    libro_id_libro_seq    SEQUENCE     �   CREATE SEQUENCE public.libro_id_libro_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 )   DROP SEQUENCE public.libro_id_libro_seq;
       public               postgres    false    220                       0    0    libro_id_libro_seq    SEQUENCE OWNED BY     I   ALTER SEQUENCE public.libro_id_libro_seq OWNED BY public.libro.id_libro;
          public               postgres    false    219            �            1259    16407    prestito    TABLE        CREATE TABLE public.prestito (
    id_prestito integer NOT NULL,
    id_libro integer NOT NULL,
    id_utente integer NOT NULL,
    data_prestito timestamp without time zone DEFAULT CURRENT_TIMESTAMP,
    data_scadenza timestamp without time zone,
    restituito boolean DEFAULT false
);
    DROP TABLE public.prestito;
       public         heap r       postgres    false            �            1259    16406    prestito_id_prestito_seq    SEQUENCE     �   CREATE SEQUENCE public.prestito_id_prestito_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 /   DROP SEQUENCE public.prestito_id_prestito_seq;
       public               postgres    false    222                       0    0    prestito_id_prestito_seq    SEQUENCE OWNED BY     U   ALTER SEQUENCE public.prestito_id_prestito_seq OWNED BY public.prestito.id_prestito;
          public               postgres    false    221            �            1259    16389    utente    TABLE     �   CREATE TABLE public.utente (
    id_utente integer NOT NULL,
    username character varying(100) NOT NULL,
    password character varying(255) NOT NULL
);
    DROP TABLE public.utente;
       public         heap r       postgres    false            �            1259    16388    utente_id_utente_seq    SEQUENCE     �   CREATE SEQUENCE public.utente_id_utente_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 +   DROP SEQUENCE public.utente_id_utente_seq;
       public               postgres    false    218                       0    0    utente_id_utente_seq    SEQUENCE OWNED BY     M   ALTER SEQUENCE public.utente_id_utente_seq OWNED BY public.utente.id_utente;
          public               postgres    false    217            f           2604    16401    libro id_libro    DEFAULT     p   ALTER TABLE ONLY public.libro ALTER COLUMN id_libro SET DEFAULT nextval('public.libro_id_libro_seq'::regclass);
 =   ALTER TABLE public.libro ALTER COLUMN id_libro DROP DEFAULT;
       public               postgres    false    220    219    220            g           2604    16410    prestito id_prestito    DEFAULT     |   ALTER TABLE ONLY public.prestito ALTER COLUMN id_prestito SET DEFAULT nextval('public.prestito_id_prestito_seq'::regclass);
 C   ALTER TABLE public.prestito ALTER COLUMN id_prestito DROP DEFAULT;
       public               postgres    false    222    221    222            e           2604    16392    utente id_utente    DEFAULT     t   ALTER TABLE ONLY public.utente ALTER COLUMN id_utente SET DEFAULT nextval('public.utente_id_utente_seq'::regclass);
 ?   ALTER TABLE public.utente ALTER COLUMN id_utente DROP DEFAULT;
       public               postgres    false    218    217    218                      0    24617    carrello 
   TABLE DATA           5   COPY public.carrello (user_id, libro_id) FROM stdin;
    public               postgres    false    223   ^'                 0    16398    libro 
   TABLE DATA           g   COPY public.libro (id_libro, titolo, autore, num_copie_tot, num_copie_disponibili, genere) FROM stdin;
    public               postgres    false    220   {'                 0    16407    prestito 
   TABLE DATA           n   COPY public.prestito (id_prestito, id_libro, id_utente, data_prestito, data_scadenza, restituito) FROM stdin;
    public               postgres    false    222   �'                 0    16389    utente 
   TABLE DATA           ?   COPY public.utente (id_utente, username, password) FROM stdin;
    public               postgres    false    218   �'                  0    0    libro_id_libro_seq    SEQUENCE SET     A   SELECT pg_catalog.setval('public.libro_id_libro_seq', 1, false);
          public               postgres    false    219                       0    0    prestito_id_prestito_seq    SEQUENCE SET     G   SELECT pg_catalog.setval('public.prestito_id_prestito_seq', 1, false);
          public               postgres    false    221                       0    0    utente_id_utente_seq    SEQUENCE SET     C   SELECT pg_catalog.setval('public.utente_id_utente_seq', 1, false);
          public               postgres    false    217            u           2606    24621    carrello carrello_pkey 
   CONSTRAINT     c   ALTER TABLE ONLY public.carrello
    ADD CONSTRAINT carrello_pkey PRIMARY KEY (user_id, libro_id);
 @   ALTER TABLE ONLY public.carrello DROP CONSTRAINT carrello_pkey;
       public                 postgres    false    223    223            q           2606    16405    libro libro_pkey 
   CONSTRAINT     T   ALTER TABLE ONLY public.libro
    ADD CONSTRAINT libro_pkey PRIMARY KEY (id_libro);
 :   ALTER TABLE ONLY public.libro DROP CONSTRAINT libro_pkey;
       public                 postgres    false    220            s           2606    16414    prestito prestito_pkey 
   CONSTRAINT     ]   ALTER TABLE ONLY public.prestito
    ADD CONSTRAINT prestito_pkey PRIMARY KEY (id_prestito);
 @   ALTER TABLE ONLY public.prestito DROP CONSTRAINT prestito_pkey;
       public                 postgres    false    222            m           2606    16394    utente utente_pkey 
   CONSTRAINT     W   ALTER TABLE ONLY public.utente
    ADD CONSTRAINT utente_pkey PRIMARY KEY (id_utente);
 <   ALTER TABLE ONLY public.utente DROP CONSTRAINT utente_pkey;
       public                 postgres    false    218            o           2606    16396    utente utente_username_key 
   CONSTRAINT     Y   ALTER TABLE ONLY public.utente
    ADD CONSTRAINT utente_username_key UNIQUE (username);
 D   ALTER TABLE ONLY public.utente DROP CONSTRAINT utente_username_key;
       public                 postgres    false    218            x           2606    24627    carrello carrello_libro_id_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.carrello
    ADD CONSTRAINT carrello_libro_id_fkey FOREIGN KEY (libro_id) REFERENCES public.libro(id_libro) ON DELETE CASCADE;
 I   ALTER TABLE ONLY public.carrello DROP CONSTRAINT carrello_libro_id_fkey;
       public               postgres    false    223    4721    220            y           2606    24622    carrello carrello_user_id_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.carrello
    ADD CONSTRAINT carrello_user_id_fkey FOREIGN KEY (user_id) REFERENCES public.utente(id_utente) ON DELETE CASCADE;
 H   ALTER TABLE ONLY public.carrello DROP CONSTRAINT carrello_user_id_fkey;
       public               postgres    false    218    223    4717            v           2606    16415    prestito prestito_id_libro_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.prestito
    ADD CONSTRAINT prestito_id_libro_fkey FOREIGN KEY (id_libro) REFERENCES public.libro(id_libro) ON DELETE CASCADE;
 I   ALTER TABLE ONLY public.prestito DROP CONSTRAINT prestito_id_libro_fkey;
       public               postgres    false    220    222    4721            w           2606    16420     prestito prestito_id_utente_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.prestito
    ADD CONSTRAINT prestito_id_utente_fkey FOREIGN KEY (id_utente) REFERENCES public.utente(id_utente) ON DELETE CASCADE;
 J   ALTER TABLE ONLY public.prestito DROP CONSTRAINT prestito_id_utente_fkey;
       public               postgres    false    218    222    4717                  x������ � �            x������ � �            x������ � �            x������ � �     