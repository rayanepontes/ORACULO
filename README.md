
# ORÁCULO

> um jogo de investigação sobre inteligência artificial, confiança e pensamento crítico.

**ORÁCULO** é um jogo solo de investigação desenvolvido em **C e Haskell**, no qual o jogador assume o papel de uma estagiária que precisa descobrir o que aconteceu dentro de um laboratório de pesquisa em Inteligência Artificial.

O problema? A principal ferramenta disponível para a investigação é justamente uma IA — e ela pode estar errada.

O jogador precisa analisar pistas, interrogar suspeitos, investigar diferentes ambientes e decidir em quais informações pode confiar. Nem toda resposta fornecida pelo sistema é verdadeira, e simplesmente seguir a maior porcentagem de confiança pode levar a uma conclusão errada.

---

## Sobre o jogo

Você é um estagiário recém-contratado pelo **NEXUS Labs**, instituto responsável pelo desenvolvimento do **ORÁCULO**, um sistema de IA utilizado para prever riscos e auxiliar na tomada de decisões.

Durante seu primeiro plantão, um alerta é disparado:

```text
ALERTA_NEXUS #0417

Dra. Elena Voss — INCONSCIENTE
Local: Sala do Servidor Central
Causa: INDETERMINADA
```

A cientista-chefe foi encontrada desacordada ao lado de um terminal onde uma exclusão de dataset foi parcialmente executada.

Existem quatro possíveis envolvidos.

Existem várias pistas.

E existe o ORÁCULO.

O problema é que nem sempre ele está dizendo a verdade.

---

## Objetivo

O objetivo não é simplesmente descobrir **quem é o culpado**.

Durante a investigação, o jogador precisa avaliar a **confiabilidade das evidências** utilizadas para chegar à conclusão.

As pistas podem ser:

* **confiáveis**
* **enviesadas**
* **alucinadas**
* **manipuladas**

O jogador deve cruzar informações, confrontar suspeitos e questionar as respostas fornecidas pelo ORÁCULO antes de realizar a acusação final.

> **confiança de uma IA não significa verdade.**

Essa é a principal ideia trabalhada pelo jogo.

---

## Mecânicas

### Investigação

O jogador pode explorar diferentes ambientes do NEXUS Labs e encontrar pistas físicas e digitais.

```text
[Recepção]
     |
[Sala de Servidores] --- [Laboratório de Dados]
     |
[Sala de Reuniões] --- [Copa]
```

### Interrogatórios

Os suspeitos podem ser interrogados durante a investigação.

O ORÁCULO pode gerar resumos dos depoimentos, mas esses resumos podem conter distorções ou informações que nunca foram mencionadas.

O jogador pode confrontar os personagens e verificar possíveis contradições.

###  ORÁCULO

O jogador pode fazer perguntas diretamente à IA.

As respostas apresentam um percentual de confiança, mas esse valor não representa necessariamente a veracidade da informação.

Isso cria uma das principais decisões do jogo:

> confiar na resposta ou investigar por conta própria?

### Minijogo de viés

No Laboratório de Dados, o jogador participa de um pequeno minijogo relacionado a **viés de dados**.

Ao trabalhar com um conjunto de dados desbalanceado, o jogador observa como isso pode influenciar uma decisão tomada pelo sistema.

### Acusação final

Ao final da investigação, o jogador deve decidir:

1. quem considera responsável;
2. quais pistas serão utilizadas como justificativa;
3. se as evidências são suficientes para sustentar a acusação.

A pontuação considera não apenas o resultado, mas também a qualidade das evidências utilizadas.

---

##  Finais

O jogo possui diferentes resultados possíveis:

| Final                                   | Resultado                                                                           |
| --------------------------------------- | ----------------------------------------------------------------------------------- |
| **A — Acusação correta e fundamentada** | O responsável é identificado utilizando evidências confiáveis.                      |
| **B — Acusação correta por sorte**      | O responsável é identificado, mas uma pista falsa foi utilizada como justificativa. |
| **C — Confiança cega**                  | O jogador segue a recomendação do ORÁCULO sem investigar suficientemente.           |
| **D — Sem provas suficientes**          | O jogador decide não acusar ninguém por falta de evidências.                        |
| **E — O ORÁCULO**                       | Final secreto relacionado à manipulação do sistema.                                 |

O jogo não considera necessariamente errado decidir **não acusar ninguém**.

Reconhecer que os dados disponíveis são insuficientes também pode ser a decisão correta.

---

##  Conceitos de Inteligência Artificial

Os conceitos de IA são apresentados dentro da própria investigação, evitando que o conteúdo educativo apareça apenas como um questionário separado.

Entre os conceitos trabalhados estão:

* viés de dados;
* alucinação de IA;
* confiança de modelos;
* dados sintéticos;
* manipulação de informações;
* limitações de sistemas de IA.

O jogador aprende esses conceitos enquanto precisa utilizá-los para resolver o caso.

---

##  Tecnologias

### C

Responsável principalmente pela estrutura e execução do jogo:

* gerenciamento do estado da investigação;
* `structs`;
* entrada e saída de dados;
* navegação entre ambientes;
* gerenciamento das pistas;
* modularização;
* persistência em arquivos.

### Haskell

Utilizado para implementar regras e funções puras relacionadas à lógica do jogo:

* classificação das pistas;
* validação das evidências;
* cálculo da acurácia investigativa;
* regras dos finais;
* processamento das decisões do jogador.

### Persistência

O jogo utiliza **arquivos locais** para armazenar o progresso da investigação.

> O projeto não utiliza banco de dados.

---

## Estrutura do projeto

A organização do código busca separar as responsabilidades do jogo entre módulos.

```text
ORÁCULO/
│
├── src/
│   ├── ...
│
├── include/
│   ├── ...
│
├── haskell/
│   ├── ...
│
├── data/
│   ├── ...
│
├── save/
│   └── ...
│
├── README.md
└── ...
```

A estrutura definitiva pode ser ajustada conforme a implementação do projeto.

---

##  Como jogar

O jogo é executado diretamente pelo terminal.

Ao iniciar, o jogador encontra o menu principal:

```text
=========================================
          N E X U S   L A B S
=========================================

              DECIFRA.IA
           PROTOCOLO SOMBRA

          [1] JOGAR
          [2] SAIR

-----------------------------------------

ORÁCULO> aguardando comando...
> _
```

A partir daí, toda a investigação acontece através de comandos no terminal.

---

##  Projeto Integrador

O **ORÁCULO** foi desenvolvido como parte do **Projeto Integrador**, integrando conceitos de programação em **C e Haskell** com uma proposta de aprendizagem sobre Inteligência Artificial.

O projeto busca unir:

**programação + narrativa + investigação + educação sobre IA**

em uma experiência interativa de terminal.

---

## Equipe

Projeto desenvolvido por:

* **[Rayane M. de Pontes Gomes - rmpg@cesar.school - rynemaria@gmail.com]**
* **[Luann Gabriel Flôr Alves da Silva - lgfas@cesar.school]**
* **[Everton Luan Gomes Batista - elgb@cesar.school]**
* **[Mirella de Sousa Albuquerque]**
* **[Marina Silva Mendes]**
* **[Anamel Thaís Ferreira Lima]**
* **[Maria Giovanna Oliveira Carvalho]**

---
