# ORÁCULO

> **"Você não joga contra a IA. Você precisa aprender a investigar a IA."**

**ORÁCULO** é um jogo solo de investigação e *escape room* em terminal desenvolvido em **C e Haskell**, integrado com uma **API de Inteligência Artificial Generativa**.

O jogador assume o papel de uma estagiária no **NEXUS Labs** que precisa descobrir o que aconteceu dentro de um laboratório de pesquisa em IA. A principal ferramenta disponível para a investigação é o próprio sistema **ORÁCULO** — mas o jogador precisará aprender que **a alta pontuação de confiança de uma IA não garante a veracidade do fato**.

---

## 🔍 Sobre o Jogo

Você é uma estagiária recém-contratada pelo **NEXUS Labs**, instituto responsável pelo desenvolvimento do **ORÁCULO**, um sistema de IA avançado utilizado para prever riscos e auxiliar na tomada de decisões.

Durante seu primeiro plantão, um alerta crítico é disparado no terminal:

```text


```



---

## 🎮 Mecânicas de Jogabilidade

```text


```

### Explorando os Ambientes

A navegação ocorre por comandos diretos no terminal entre diferentes salas interconectadas do NEXUS Labs:

```text
        [Recepção]
            |
  [Sala de Servidores] ─── [Laboratório de Dados]
            |
  [Sala de Reuniões]  ─── [Copa]

```


---

## 🧠 Conceitos de IA Ensinados

O jogo integra a teoria da Inteligência Artificial diretamente na solução do caso:

| Conceito de IA | Como se Aplica no Jogo | Mecânica Investigativa |
| --- | --- | --- |
| **Alucinação de IA** | A IA gera relatórios convincentes citando arquivos que não existem no servidor. | O jogador deve buscar a fonte primária e apontar a alucinação para descartar a pista falsa. |
| **Viés Algorítmico** | Perfis de acesso de dois funcionários idênticos recebem classificações de risco diferentes. | Análise das métricas do modelo no Laboratório de Dados para identificar dados históricos tendenciosos. |
| **Prompt Injection** | Instruções maliciosas foram enviadas ao terminal da Dra. Voss. | Identificar comandos ocultos enviados à IA para forçar a liberação de senhas ou logs. |
| **Confiabilidade de Modelos** | Uma resposta da IA possui 98% de confiança, mas é desmentida por um log físico. | Julgamento crítico sobre as métricas de saída do modelo. |
| **Dados Sintéticos / Incompletos** | O dataset utilizado para prever falhas omitiu logs do turno da noite. | Identificar falhas na amostragem que levaram a conclusões equivocadas. |


---

## 🏗️ Arquitetura & Tecnologias

O sistema combina a eficiência de execução do **C** com a robustez funcional do **Haskell**, além do apoio da **API de IA Generativa**.

```text
┌──────────────────────────────────────┐     ┌──────────────────────────────────────┐
│           CORE EM C (ENGINE)         │     │        LÓGICA EM HASKELL (RULES)     │
├──────────────────────────────────────┤     ├──────────────────────────────────────┤
│ • Interface de terminal e menus      │     │ • Classificação e validação de pistas│
│ • Controle de salas e inventário     │  ─> │ • Validação do rigor investigativo   │
│ • Cronômetro e chamadas HTTP (libcurl│     │ • Cálculo da acurácia e pontuação    │
│ • Persistência local em arquivos     │     │ • Árvore de decisão dos Finais (A-E) │
└──────────────────────────────────────┘     └──────────────────────────────────────┘
                   │
                   ▼
┌──────────────────────────────────────────────────────────────────────────────────┐
│                             GERAÇÃO PROCEDURAL & FALLBACK                        │
├──────────────────────────────────────────────────────────────────────────────────┤
│ • API de IA Generativa: Geração dinâmica do enredo, suspeitos e JSON da partida  │
│ • Arquivos Locais (Fallback): Casos pré-definidos caso a API esteja offline     │
└──────────────────────────────────────────────────────────────────────────────────┘

```

### Regra de Arquitetura

A IA Generativa atua apenas na **criação de conteúdo e narrativa**. O controle de regras, tempo, estado do jogo, cálculo de pontos e condições de vitória/derrota é feito de forma determinística pelo motor em **C e Haskell**.

---

## 📁 Estrutura do Repositório
---

---

## 🎓 Projeto Integrador

O **ORÁCULO** foi desenvolvido como parte do **Projeto Integrador**, combinando técnicas avançadas de programação estruturada (**C**), programação funcional (**Haskell**) e **IA Generativa**, com uma proposta narrativa voltada para o letramento e pensamento crítico em Inteligência Artificial.

---

## 📜 Licença

Este projeto está licenciado sob a licença **MIT** — consulte o arquivo [LICENSE](https://www.google.com/search?q=LICENSE) para obter mais detalhes.

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
