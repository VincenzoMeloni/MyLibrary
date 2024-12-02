--
-- PostgreSQL database dump
--

-- Dumped from database version 17.1 (Ubuntu 17.1-1.pgdg24.04+1)
-- Dumped by pg_dump version 17.1 (Ubuntu 17.1-1.pgdg24.04+1)

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET transaction_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- Name: checkout_trigger_function(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION public.checkout_trigger_function() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN
    INSERT INTO prestito (id_utente, id_libro, data_prestito, data_scadenza, restituito)
    VALUES (NEW.user_id, NEW.libro_id, NOW(), NOW() + INTERVAL '14 days', FALSE);

    UPDATE libro
    SET num_copie_disponibili = num_copie_disponibili - NEW.quantita
    WHERE id_libro = NEW.libro_id AND num_copie_disponibili >= NEW.quantita;

    UPDATE utente
    SET max_prestiti = max_prestiti - NEW.quantita
    WHERE id_utente = NEW.user_id AND max_prestiti >= NEW.quantita;

    DELETE FROM carrello
    WHERE user_id = NEW.user_id AND libro_id = NEW.libro_id;

    RETURN NEW;
END;
$$;


ALTER FUNCTION public.checkout_trigger_function() OWNER TO postgres;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- Name: carrello; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.carrello (
    user_id integer NOT NULL,
    libro_id integer NOT NULL,
    is_checkout boolean DEFAULT false,
    quantita integer DEFAULT 1
);


ALTER TABLE public.carrello OWNER TO postgres;

--
-- Name: libro; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.libro (
    id_libro integer NOT NULL,
    titolo character varying(200) NOT NULL,
    autore character varying(200) NOT NULL,
    num_copie_tot integer NOT NULL,
    num_copie_disponibili integer NOT NULL,
    genere character varying(255),
    CONSTRAINT libro_num_copie_disponibili_check CHECK ((num_copie_disponibili >= 0)),
    CONSTRAINT libro_num_copie_tot_check CHECK ((num_copie_tot >= 0))
);


ALTER TABLE public.libro OWNER TO postgres;

--
-- Name: libro_id_libro_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.libro_id_libro_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.libro_id_libro_seq OWNER TO postgres;

--
-- Name: libro_id_libro_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.libro_id_libro_seq OWNED BY public.libro.id_libro;


--
-- Name: prestito; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.prestito (
    id_prestito integer NOT NULL,
    id_libro integer NOT NULL,
    id_utente integer NOT NULL,
    data_prestito timestamp without time zone DEFAULT CURRENT_TIMESTAMP,
    data_scadenza timestamp without time zone,
    restituito boolean DEFAULT false
);


ALTER TABLE public.prestito OWNER TO postgres;

--
-- Name: prestito_id_prestito_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.prestito_id_prestito_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.prestito_id_prestito_seq OWNER TO postgres;

--
-- Name: prestito_id_prestito_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.prestito_id_prestito_seq OWNED BY public.prestito.id_prestito;


--
-- Name: utente; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.utente (
    id_utente integer NOT NULL,
    username character varying(100) NOT NULL,
    password character varying(255) NOT NULL,
    max_prestiti integer DEFAULT 5
);


ALTER TABLE public.utente OWNER TO postgres;

--
-- Name: utente_id_utente_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.utente_id_utente_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.utente_id_utente_seq OWNER TO postgres;

--
-- Name: utente_id_utente_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.utente_id_utente_seq OWNED BY public.utente.id_utente;


--
-- Name: libro id_libro; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.libro ALTER COLUMN id_libro SET DEFAULT nextval('public.libro_id_libro_seq'::regclass);


--
-- Name: prestito id_prestito; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.prestito ALTER COLUMN id_prestito SET DEFAULT nextval('public.prestito_id_prestito_seq'::regclass);


--
-- Name: utente id_utente; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.utente ALTER COLUMN id_utente SET DEFAULT nextval('public.utente_id_utente_seq'::regclass);


--
-- Data for Name: carrello; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.carrello (user_id, libro_id, is_checkout, quantita) FROM stdin;
\.


--
-- Data for Name: libro; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.libro (id_libro, titolo, autore, num_copie_tot, num_copie_disponibili, genere) FROM stdin;
3	Dune	Frank Herbert	5	3	Sci-Fi
5	Il Nome della Rosa	Umberto Eco	6	4	Storia
2	Il Silenzio degli Innocenti	Thomas Harris	7	3	Thriller
1	Il Signore degli Anelli	J.R.R. Tolkien	10	5	Fantasy
4	Orgoglio e Pregiudizio	Jane Austen	8	4	Romanzo
\.


--
-- Data for Name: prestito; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.prestito (id_prestito, id_libro, id_utente, data_prestito, data_scadenza, restituito) FROM stdin;
16	4	1	2024-12-02 19:09:21.256714	2024-12-16 19:09:21.256714	f
17	4	1	2024-12-02 19:19:18.785382	2024-12-16 19:19:18.785382	f
\.


--
-- Data for Name: utente; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.utente (id_utente, username, password, max_prestiti) FROM stdin;
2	marco	merrino	5
4	silvia	meloni	5
8	ivano	meliva	3
3	danny	lazzarin	1
1	vincenzo	a	0
\.


--
-- Name: libro_id_libro_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.libro_id_libro_seq', 5, true);


--
-- Name: prestito_id_prestito_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.prestito_id_prestito_seq', 17, true);


--
-- Name: utente_id_utente_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.utente_id_utente_seq', 8, true);


--
-- Name: carrello carrello_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.carrello
    ADD CONSTRAINT carrello_pkey PRIMARY KEY (user_id, libro_id);


--
-- Name: libro libro_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.libro
    ADD CONSTRAINT libro_pkey PRIMARY KEY (id_libro);


--
-- Name: prestito prestito_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.prestito
    ADD CONSTRAINT prestito_pkey PRIMARY KEY (id_prestito);


--
-- Name: utente utente_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.utente
    ADD CONSTRAINT utente_pkey PRIMARY KEY (id_utente);


--
-- Name: utente utente_username_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.utente
    ADD CONSTRAINT utente_username_key UNIQUE (username);


--
-- Name: carrello checkout_trigger; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER checkout_trigger AFTER UPDATE OF is_checkout ON public.carrello FOR EACH ROW WHEN ((new.is_checkout = true)) EXECUTE FUNCTION public.checkout_trigger_function();


--
-- Name: carrello carrello_libro_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.carrello
    ADD CONSTRAINT carrello_libro_id_fkey FOREIGN KEY (libro_id) REFERENCES public.libro(id_libro) ON DELETE CASCADE;


--
-- Name: carrello carrello_user_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.carrello
    ADD CONSTRAINT carrello_user_id_fkey FOREIGN KEY (user_id) REFERENCES public.utente(id_utente) ON DELETE CASCADE;


--
-- Name: prestito prestito_id_libro_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.prestito
    ADD CONSTRAINT prestito_id_libro_fkey FOREIGN KEY (id_libro) REFERENCES public.libro(id_libro) ON DELETE CASCADE;


--
-- Name: prestito prestito_id_utente_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.prestito
    ADD CONSTRAINT prestito_id_utente_fkey FOREIGN KEY (id_utente) REFERENCES public.utente(id_utente) ON DELETE CASCADE;


--
-- PostgreSQL database dump complete
--

