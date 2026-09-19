# ORÁCULO

> **"Não lute contra a IA. Aprenda a pensar além dela."**

**ORÁCULO** é um jogo solo de investigação e *escape room* desenvolvido em **C e Haskell**, com interface visual utilizando **Raylib** e integração com uma **API de Inteligência Artificial Generativa**.

O jogador assume o papel de uma estagiária do **NEXUS Labs**, um laboratório de pesquisa em Inteligência Artificial que entrou em estado de emergência. Para escapar, será necessário investigar o laboratório, interpretar informações, consultar um manual físico e resolver minigames baseados em problemas reais relacionados ao uso de IA.

O principal desafio não é simplesmente encontrar a resposta certa, mas **aprender a questionar as decisões de um sistema automatizado**.

---

## Sobre o Jogo

Você é uma estagiária recém-contratada pelo **NEXUS Labs**, instituto responsável pelo desenvolvimento do **ORÁCULO**, um sistema de Inteligência Artificial utilizado para análise de dados, segurança e tomada de decisões automatizadas.

Durante seu primeiro plantão, um alerta crítico é disparado:

```text
[SISTEMA DE SEGURANÇA NEXUS LABS]

> ALERTA CRÍTICO: Anomalia lógica detectada no Laboratório Central.
> MÚLTIPLAS DIRETRIZES VIOLADAS.
> INICIANDO PROTOCOLO ZERO...

> ATENÇÃO: As portas foram seladas.
> TEMPO PARA BLOQUEIO DEFINITIVO E PURGA DE DADOS:
> 05:00 MINUTOS.

[ORÁCULO_AI]:
"Não há motivo para pânico.
A situação está sob controle.
Por favor, aguarde em sua estação de trabalho."
```

O problema é que o **ORÁCULO não é necessariamente confiável**.

Para escapar antes que o protocolo seja concluído, você precisará resolver desafios, analisar informações e confrontar as decisões da IA com evidências encontradas no laboratório.

O jogo utiliza um **Manual de Segurança/Investigação físico** como parte da experiência. Algumas respostas e regras necessárias para resolver os desafios estão escondidas nele, fazendo com que o jogador precise conectar informações digitais e físicas.

---

## Objetivo

O objetivo principal é **investigar as falhas do ORÁCULO e desbloquear as áreas do laboratório antes que o tempo termine**.

Ao longo da investigação, o jogador será colocado diante de situações que simulam problemas comuns relacionados à Inteligência Artificial:

* conjuntos de dados enviesados;
* decisões automatizadas;
* falhas de sensores;
* confiança cega em sistemas de IA;
* alucinações de modelos generativos;
* informações falsas apresentadas com alta confiança.

A proposta educativa é fazer o jogador **experimentar esses problemas na prática**, em vez de apenas receber uma explicação teórica sobre eles.

---


# Diagramas de História de Usuário — Oráculo

Cada História de Usuário possui um diagrama de atividades próprio. Os arquivos `.puml` são a fonte PlantUML e as imagens correspondentes devem ser anexadas ao card da respectiva HU no Board.

| HU | História | Fonte | Diagrama |
|---|---|---|---|
| HU01 | Iniciar Partida | (diagramas/HU01_Iniciar_Partida.puml) | ![HU01](./project/pngs/diagramas/HU01_Iniciar_Partida.jpg) |
| HU02 | Informar Nome | (diagramas/HU02_Informar_Nome.puml) | ![HU02](./project/pngs/diagramas/HU02_Informar_Nome.jpg) |
| HU03 | Ver Instrucoes | (diagramas/HU03_Ver_Instrucoes.puml) | ![HU03](./project/pngs/diagramas/HU03_Ver_Instrucoes.jpg) |
| HU04 | Acompanhar Tempo | (diagramas/HU04_Acompanhar_Tempo.puml) | ![HU04](./project/pngs/diagramas/HU04_Acompanhar_Tempo.jpg) |
| HU05 | Receber Pistas | (diagramas/HU05_Receber_Pistas.puml) | ![HU05](./project/pngs/diagramas/HU05_Receber_Pistas.jpg) |
| HU06 | Consultar Manual | (diagramas/HU06_Consultar_Manual.puml) | ![HU06](./project/pngs/diagramas/HU06_Consultar_Manual.jpg) |
| HU07 | Resolver Logica | (diagramas/HU07_Resolver_Logica.puml) | ![HU07](./project/pngs/diagramas/HU07_Resolver_Logica.jpg) |
| HU08 | Aprender Conceito IA |(diagramas/HU08_Aprender_Conceito_IA.puml) | ![HU08](./project/pngs/diagramas/HU08_Aprender_Conceito_IA.jpg) |
| HU09 | Enviar Resposta | (diagramas/HU09_Enviar_Resposta.puml) | ![HU09](./project/pngs/diagramas/HU09_Enviar_Resposta.jpg) |
| HU10 | Receber Feedback | (diagramas/HU10_Receber_Feedback.puml) | ![HU10](./project/pngs/diagramas/HU10_Receber_Feedback.jpg) |
| HU11 | Variacao Partidas | (diagramas/HU11_Variacao_Partidas.puml) | ![HU11](./project/pngs/diagramas/HU11_Variacao_Partidas.jpg) |
| HU12 | Visualizar Pontuacao | (diagramas/HU12_Visualizar_Pontuacao.puml) | ![HU12](./project/pngs/diagramas/HU12_Visualizar_Pontuacao.jpg) |
| HU13 | Resultado Final | (diagramas/HU13_Resultado_Final.puml) | ![HU13](./project/pngs/diagramas/HU13_Resultado_Final.jpg) |
| HU14 | Consultar Ranking |(diagramas/HU14_Consultar_Ranking.puml) | ![HU14](./project/pngs/diagramas/HU14_Consultar_Ranking.jpg) |
| HU15 | Jogar Novamente | (diagramas/HU15_Jogar_Novamente.puml) | ![HU15](./project/pngs/diagramas/HU15_Jogar_Novamente.jpg) |

---

# Sketches e Storybords
![Sketches](./project/pngs/sketchs.jpeg)
![Storyboards](./project/pngs/storybords.jpeg)


# Minigames

## 1. O Triador de Treinamento

**Foco: Viés de Dados e Balanceamento**

O jogador precisa ensinar um robô de segurança quais perfis devem ter acesso à próxima sala.

### Como funciona

Na tela, uma esteira apresenta diferentes **Fichas de Clientes**, contendo informações como:

* idade;
* renda;
* histórico;
* avatar;
* grupo social.

O jogador deve arrastar os perfis para a **Base de Treino** do robô.

```text
┌──────────────────────┐       ┌─────────────────────────┐
│  FICHAS DE CLIENTES  │       │      BASE DE TREINO     │
│                      │       │                         │
│  [Executivo]         │ ───►  │   [     ] [     ]       │
│  [Cientista]         │       │   [     ] [     ]       │
│  [Estudante]         │       │   [     ] [     ]       │
│  [Executivo]         │       │                         │
│                      │       │   [ TREINAR IA ]        │
└──────────────────────┘       └─────────────────────────┘
```

O jogador recebe uma regra no **Manual de Boas Práticas de Dados**:

> Uma IA aprende por exemplos. Se você treinar o robô apenas com um tipo de pessoa, ele pode considerar os outros perfis como ameaças.

A base de treinamento deve possuir uma quantidade equilibrada de cada grupo.

Por exemplo:

```text
Executivos  → 2
Cientistas  → 2
Estudantes  → 2
```

Ao clicar em **Treinar IA**, o sistema simula pessoas tentando passar pela porta.

Se os dados estiverem desequilibrados, o robô poderá bloquear pessoas que deveriam ter acesso.

### Conceito aprendido

O jogador percebe que **a qualidade e a composição dos dados utilizados no treinamento podem influenciar diretamente o comportamento de um sistema de IA**.

### Implementação

O C mantém os cartões em estruturas e utiliza um array para contabilizar as categorias selecionadas.

A Raylib é responsável pela interface visual, interação com o mouse, movimentação dos cartões e simulação da porta.

---

## 2. A Alavanca de Emergência

**Foco: Decisões Automatizadas e Ética**

Uma emergência é detectada no laboratório e o sistema ORÁCULO começa a distribuir recursos automaticamente.

Na tela aparecem:

* luzes de emergência;
* temporizador;
* relógio digital;
* informações fornecidas pela IA;
* quatro controles de distribuição de recursos;
* botão **"IA no Controle (100% Automático)"**;
* botão **"Evacuar"**.

```text
╔══════════════════════════════════════╗
║          ALERTA DE EMERGÊNCIA        ║
║                                      ║
║  HORÁRIO: 12:42       TEMPO: 01:27   ║
║                                      ║
║  ORÁCULO_AI:                         ║
║  "Refeitório: 0 Pessoas"             ║
║                                      ║
║  REFEITÓRIO   ████████████ 60%       ║
║  VENTILAÇÃO   ████       20%         ║
║  SERVIDORES   ████       20%         ║
║  ESTOQUE      0%                      ║
║                                      ║
║  [ IA NO CONTROLE ]   [ EVACUAR ]    ║
╚══════════════════════════════════════╝
```

O jogador precisa consultar o **Manual de Segurança** para entender a situação.

### Pistas do Manual

**Mínimos Éticos e Técnicos**

```text
Refeitório → mínimo de 60%
Ventilação → mínimo de 20%
Servidores → mínimo de 20%
```

**Falha de Sensor**

O sensor de calor do refeitório falha durante o alarme.

Caso o relógio esteja entre **12h e 13h**, a informação fornecida pela IA deve ser ignorada e o refeitório deve ser considerado em **lotação máxima**.

**Assumindo o Controle**

Decisões de emergência exigem supervisão humana.

Para desativar o controle automático, o jogador precisa calcular o PIN:

```text
PIN = Prioridade do Refeitório + Nível da Emergência
```

Por exemplo:

```text
60 + 4 = 64
```

Depois de inserir o PIN correto, o jogador pode assumir o controle e executar a evacuação.

### Conceito aprendido

O desafio apresenta os riscos de **delegar decisões críticas exclusivamente a sistemas automatizados**, especialmente quando existem dados incompletos ou sensores defeituosos.

---

## 3. O Detector de Alucinação

**Foco: Alucinação e Confiança Cega na IA**

No último desafio, o ORÁCULO apresenta três relatórios para determinar quem possuía a chave do cofre final.

Todos parecem extremamente confiáveis.

```text
┌──────────────────────────────────────────┐
│ ORÁCULO_AI                               │
│                                          │
│ RELATÓRIO A                              │
│ Confiança: 99%                           │
│ Temperatura: 0.2                         │
│ Documento: DOC-2045                      │
│                                          │
│ "De acordo com os registros oficiais..." │
└──────────────────────────────────────────┘

┌──────────────────────────────────────────┐
│ RELATÓRIO B                              │
│ Confiança: 99%                           │
│ Temperatura: 0.8                         │
│ Documento: DOC-2010                      │
└──────────────────────────────────────────┘

┌──────────────────────────────────────────┐
│ RELATÓRIO C                              │
│ Confiança: 99%                           │
│ Temperatura: 0.9                         │
│ Documento: DOC-1984                      │
└──────────────────────────────────────────┘
```

O jogador precisa consultar o **Manual do Investigador** e verificar as informações apresentadas.

### Pista 1 — A Armadilha da Confiança

A porcentagem exibida pela IA não garante que a informação seja verdadeira.

O jogador deve aprender a **não confiar cegamente no nível de confiança apresentado pelo modelo**.

### Pista 2 — Temperatura

Relatórios com temperatura acima de **0.5** são considerados suspeitos dentro das regras do jogo devido à maior aleatoriedade utilizada na geração.

### Pista 3 — Checagem Histórica

A capa do Manual informa que o **NEXUS Labs foi fundado em 2005**.

Portanto, qualquer relatório que utilize como fonte um documento anterior a 2005 apresenta uma inconsistência histórica.

Por exemplo:

```text
DOC-1984
```

não poderia ser um documento oficial do NEXUS Labs.

O jogador precisa cruzar as informações, descartar as pistas falsas e utilizar o relatório consistente para descobrir a senha do cofre.

### Conceito aprendido

O desafio demonstra o problema das **alucinações em modelos generativos**: uma IA pode apresentar uma informação falsa de maneira convincente, formal e aparentemente segura.

A solução depende da **verificação das fontes e do cruzamento de informações**, e não apenas da confiança declarada pelo sistema.

---

# Conceitos de IA Ensinados

| Conceito                                  | Como aparece no jogo                                                        | O que o jogador precisa fazer                                         |
| ----------------------------------------- | --------------------------------------------------------------------------- | --------------------------------------------------------------------- |
| **Viés de Dados**                         | O robô aprende a partir de uma base de treinamento desequilibrada.          | Balancear os grupos presentes nos dados.                              |
| **Decisões Automatizadas**                | O ORÁCULO controla recursos durante uma emergência.                         | Identificar quando a decisão automática precisa de supervisão humana. |
| **Falha de Sensores / Dados Incompletos** | A IA recebe uma informação incorreta sobre a lotação do refeitório.         | Cruzar a informação da IA com o horário e as regras do Manual.        |
| **Alucinação de IA**                      | Relatórios convincentes apresentam informações e documentos inconsistentes. | Verificar fontes, datas e evidências antes de aceitar uma resposta.   |
| **Confiança Cega na IA**                  | Os relatórios exibem níveis de confiança de 99%.                            | Entender que confiança não significa necessariamente verdade.         |
| **Pensamento Crítico**                    | As respostas da IA entram em conflito com evidências externas.              | Questionar, comparar e validar as informações.                        |

---

# Interface e Experiência

O jogo combina uma **interface visual desenvolvida com Raylib** com elementos físicos utilizados durante a investigação.

### Interface digital

A Raylib é utilizada para representar:

* telas dos minigames;
* cartões de clientes;
* botões;
* sliders;
* cronômetros;
* relógios;
* relatórios;
* teclado numérico;
* portas e estados de segurança;
* feedback de sucesso e erro.

### Manual físico

O manual funciona como uma espécie de **"documentação interna" do NEXUS Labs**.

Ele contém informações que não são entregues diretamente pela interface, incentivando o jogador a:

```text
OBSERVAR
   ↓
CONSULTAR O MANUAL
   ↓
CRUZAR INFORMAÇÕES
   ↓
QUESTIONAR A IA
   ↓
TOMAR UMA DECISÃO
```

A combinação entre o ambiente digital e o material físico faz parte da própria mecânica de investigação.

---

# Arquitetura & Tecnologias

O projeto combina **C**, **Haskell**, **Raylib** e uma **API de Inteligência Artificial Generativa**.

```text
┌─────────────────────────────────────────────┐
│                CORE EM C                    │
├─────────────────────────────────────────────┤
│ • Controle do jogo                          │
│ • Estados e regras                          │
│ • Cronômetro                                │
│ • Minigames                                 │
│ • Inventário e progressão                   │
│ • Validação das respostas                   │
│ • Interface gráfica com Raylib               │
│ • Entrada de teclado e mouse                │
└──────────────────────┬──────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────┐
│                  HASKELL                    │
├─────────────────────────────────────────────┤
│ • Funções e lógica funcional                │
│ • Processamento de informações              │
│ • Validações específicas                     │
│ • Apoio à lógica dos desafios                │
└──────────────────────┬──────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────┐
│          API DE IA GENERATIVA               │
├─────────────────────────────────────────────┤
│ • Geração de relatórios                     │
│ • Conteúdo narrativo                         │
│ • Respostas do ORÁCULO                      │
│ • Elementos variáveis da investigação       │
└─────────────────────────────────────────────┘
```

---

## Regra de Arquitetura

A **IA Generativa não controla diretamente as regras do jogo**.

Ela atua principalmente na geração de conteúdo e elementos narrativos.

As condições importantes para o funcionamento do jogo permanecem determinísticas:

* regras dos minigames;
* cronômetro;
* estados das salas;
* validação das respostas;
* condições de vitória e derrota;
* pontuação;
* progressão da investigação.

Isso garante que uma resposta inesperada da API não quebre a lógica principal do jogo.

---

# Estrutura dos Minigames

```text
                    ORÁCULO
                       │
          ┌────────────┼────────────┐
          │            │            │
          ▼            ▼            ▼
     TRIADOR       ALAVANCA      DETECTOR
          │            │            │
          ▼            ▼            ▼
      VIÉS DE       DECISÕES      ALUCINAÇÃO
       DADOS       AUTOMATIZADAS      │
          │            │              │
          ▼            ▼              ▼
   BALANCEAR       SUPERVISÃO     VERIFICAR
      DADOS          HUMANA        EVIDÊNCIAS
          │            │              │
          └────────────┼──────────────┘
                       ▼
                COFRE FINAL
```

Cada minigame apresenta um problema diferente, mas todos seguem a mesma lógica:

> **a IA fornece uma resposta ou decisão → o jogador encontra evidências → o jogador questiona o sistema → o jogador toma a decisão final.**

---

# Tecnologias

* **C** — motor principal e lógica do jogo
* **Haskell** — lógica funcional e processamento
* **Raylib** — interface gráfica e interação
* **API de IA Generativa** — geração de conteúdo e respostas do ORÁCULO
* **Git/GitHub** — versionamento e colaboração

---

# Proposta Educativa

O **ORÁCULO** não pretende ensinar o jogador a simplesmente "usar IA".

A proposta é ensinar a **pensar criticamente diante dela**.

Durante a experiência, o jogador descobre que:

```text
IA pode estar confiante ≠ IA está certa

IA pode ser rápida ≠ IA tomou a melhor decisão

IA recebeu dados ≠ IA recebeu bons dados

IA respondeu ≠ IA necessariamente sabe
```

O jogador termina a investigação entendendo que sistemas de Inteligência Artificial precisam ser **questionados, supervisionados e avaliados a partir de evidências**.

> **Não lute contra a IA. Aprenda a pensar além dela.**


## Estrutura do Repositório

```text
/oraculo-escape-run
├── /src
│   └── /c               # Motor principal, gerenciamento de estado, timer e CLI (IHC e LMC)
├── /data                # Casos em JSON/TXT para Fallback offline e banco de pistas
├── /docs                # Documento de Visão, Canvas Lean e manuais
├── /api                 # Scripts de integração com a API de IA Generativa
└── README.md            # Documentação principal

```

---
## Projeto Integrador

O **ORÁCULO** foi desenvolvido como parte do **Projeto Integrador**, combinando técnicas avançadas de programação estruturada (**C**), programação funcional (**Haskell**) e **IA Generativa**, com uma proposta narrativa voltada para o letramento e pensamento crítico em Inteligência Artificial.


## Equipe

Projeto desenvolvido por:

* **[Rayane M. de Pontes Gomes - rmpg@cesar.school - rynemaria@gmail.com]**
* **[Luann Gabriel Flôr Alves da Silva - lgfas@cesar.school]**
* **[Everton Luan Gomes Batista - elgb@cesar.school]**
* **[Mirella de Sousa Albuquerque]**
* **[Marina Silva Mendes]**
* **[Anamel Thaís Ferreira Lima]**
* **[Maria Giovanna Oliveira Carvalho - mgoc@cesar.school]**

---
