# Histórias de Usuário — ORÁCULO

## HU01 — Iniciar uma nova partida

**Card:**  
Como jogador, quero iniciar uma nova partida, para começar uma investigação no Oráculo.

**Conversation:**  
Ao selecionar **“Iniciar”**, o sistema deve preparar a partida, apresentar brevemente o contexto e iniciar o fluxo do jogo. O jogador deve entender rapidamente o que precisa fazer.

**Confirmation:**
- [ ] O jogador consegue iniciar uma partida pelo menu principal.
- [ ] O contexto inicial é apresentado.
- [ ] O jogo segue para o primeiro desafio sem exigir conhecimento prévio.

---

## HU02 — Identificar-me antes da partida

**Card:**  
Como jogador, quero informar meu nome ou apelido, para identificar meu resultado no ranking.

**Conversation:**  
Antes da partida, o jogador informa uma identificação curta. Esse nome será associado à pontuação e ao tempo obtidos.

**Confirmation:**
- [ ] O sistema permite inserir um nome ou apelido válido.
- [ ] A identificação é associada ao resultado final.
- [ ] O mesmo nome ou apelido aparece no ranking.

---

## HU03 — Compreender rapidamente como jogar

**Card:**  
Como jogador, quero receber instruções simples antes da partida, para entender a dinâmica sem gastar muito tempo aprendendo as regras.

**Conversation:**  
As instruções devem explicar de forma curta que o jogador receberá pistas na tela, consultará o Manual do Oráculo e responderá aos desafios dentro do limite de tempo.

**Confirmation:**
- [ ] As instruções apresentam o objetivo da partida.
- [ ] O limite de 5 minutos é informado.
- [ ] O uso do manual é explicado.
- [ ] A forma de responder aos desafios é apresentada.

---

## HU04 — Acompanhar o tempo restante

**Card:**  
Como jogador, quero visualizar o tempo restante, para administrar minhas decisões durante a investigação.

**Conversation:**  
A partida terá duração máxima de 5 minutos. O cronômetro deve permanecer visível e criar sensação de urgência sem atrapalhar a leitura.

**Confirmation:**
- [ ] O cronômetro começa em `05:00`.
- [ ] O tempo diminui durante a partida.
- [ ] O cronômetro permanece visível.
- [ ] A partida é encerrada ao chegar a `00:00`.

---

## HU05 — Receber pistas da investigação

**Card:**  
Como jogador, quero receber pistas durante a partida, para descobrir como solucionar o problema apresentado pelo Oráculo.

**Conversation:**  
As pistas podem apresentar situações, mensagens, códigos ou informações relacionadas à IA. Elas não devem entregar diretamente a resposta.

**Confirmation:**
- [ ] O sistema apresenta pistas relacionadas ao desafio atual.
- [ ] As pistas são compreensíveis.
- [ ] As informações apresentadas ajudam o jogador a chegar à solução sem entregar diretamente a resposta.

---

## HU06 — Consultar o Manual do Oráculo

**Card:**  
Como jogador, quero consultar o Manual do Oráculo, para encontrar informações que me ajudem a interpretar as pistas.

**Conversation:**  
Algumas respostas não poderão ser descobertas apenas olhando a tela. O jogador deverá relacionar a pista exibida com conceitos, protocolos, códigos ou tabelas existentes no manual físico.

**Confirmation:**
- [ ] Pelo menos um desafio exige uma informação disponível no manual.
- [ ] A informação necessária pode ser localizada no manual.
- [ ] A consulta permite chegar à resposta sem exigir conhecimento prévio do assunto.

---

## HU07 — Resolver desafio de lógica

**Card:**  
Como jogador, quero interpretar símbolos de lógica com auxílio do manual, para resolver um dos desafios da investigação mesmo sem conhecer lógica previamente.

**Conversation:**  
O jogo poderá utilizar proposições como `P`, `Q`, `∧`, `∨`, `¬` e `↔`. O manual deverá apresentar uma explicação simples dos símbolos necessários.

**Confirmation:**
- [ ] O desafio pode ser resolvido consultando o manual.
- [ ] Não é necessário conhecimento prévio de lógica.
- [ ] O desafio possui uma resposta objetiva que pode ser validada pelo sistema.

---

## HU08 — Aprender conceitos de IA pelas pistas

**Card:**  
Como jogador, quero encontrar conceitos de Inteligência Artificial durante a investigação, para aprender enquanto tento solucionar o caso.

**Conversation:**  
Conceitos como alucinação, viés algorítmico e segurança de IA devem aparecer dentro das situações do jogo, e não como uma prova tradicional.

**Confirmation:**
- [ ] Pelo menos um conceito de IA está integrado a um desafio.
- [ ] O conceito é apresentado de maneira acessível.
- [ ] Compreender o conceito ajuda o jogador a encontrar a resposta.

---

## HU09 — Enviar uma resposta

**Card:**  
Como jogador, quero informar minha resposta para um desafio, para verificar se minha interpretação está correta e continuar a investigação.

**Conversation:**  
Depois de analisar a tela e consultar o manual, o jogador deverá conseguir selecionar ou digitar uma resposta.

**Confirmation:**
- [ ] O sistema recebe a resposta do jogador.
- [ ] A resposta é validada.
- [ ] O fluxo da partida continua de acordo com o resultado.

---

## HU10 — Receber feedback das respostas

**Card:**  
Como jogador, quero receber feedback após responder, para saber imediatamente se acertei ou errei.

**Conversation:**  
O feedback deve ser rápido e claro. Ele pode utilizar texto, efeitos visuais e/ou sonoros, sem interromper excessivamente o ritmo da partida.

**Confirmation:**
- [ ] Uma resposta correta gera indicação de acerto.
- [ ] Uma resposta incorreta gera indicação de erro.
- [ ] O jogador consegue compreender imediatamente o resultado.

---

## HU11 — Ter variação entre partidas

**Card:**  
Como jogador, quero encontrar variações nos desafios entre as partidas, para poder jogar novamente sem ter exatamente a mesma experiência.

**Conversation:**  
O jogo possuirá um banco de pistas e desafios previamente criados. O sistema poderá selecionar ou organizar parte deles aleatoriamente.

**Confirmation:**
- [ ] Partidas diferentes podem apresentar diferenças na seleção ou ordem dos desafios.
- [ ] A variação não prejudica a coerência da investigação.
- [ ] O sistema utiliza apenas desafios previamente definidos pela equipe no MVP.

---

## HU12 — Visualizar minha pontuação

**Card:**  
Como jogador, quero acompanhar minha pontuação, para saber como estou me saindo durante a partida.

**Conversation:**  
Resolver desafios corretamente deve contribuir para a pontuação. A pontuação deve valorizar acertos e desencorajar respostas aleatórias.

**Confirmation:**
- [ ] O jogador ganha pontos conforme as regras definidas.
- [ ] A pontuação é atualizada corretamente.
- [ ] O resultado final reflete o desempenho do jogador.

---

## HU13 — Visualizar meu resultado final

**Card:**  
Como jogador, quero visualizar meu resultado ao terminar a partida, para entender meu desempenho na investigação.

**Conversation:**  
Ao concluir a investigação ou quando o tempo acabar, deverá ser apresentada uma tela final contendo informações relevantes da partida.

**Confirmation:**
- [ ] O encerramento apresenta o nome ou apelido do jogador.
- [ ] A pontuação final é exibida.
- [ ] O tempo de conclusão é mostrado ou o sistema informa que o tempo se esgotou.

---

## HU14 — Consultar o ranking

**Card:**  
Como jogador, quero consultar o ranking de participantes, para comparar meu desempenho com o de outros jogadores.

**Conversation:**  
O ranking será local. A pontuação será o critério principal e o menor tempo funcionará como critério de desempate.

**Confirmation:**
- [ ] Os resultados são armazenados localmente.
- [ ] O ranking pode ser consultado.
- [ ] Os participantes são ordenados pela pontuação.
- [ ] Em caso de empate na pontuação, o menor tempo é utilizado como desempate.

---

## HU15 — Jogar novamente

**Card:**  
Como jogador, quero iniciar outra partida após terminar, para tentar melhorar minha pontuação e meu tempo.

**Conversation:**  
A experiência deve incentivar a rejogabilidade. Depois do resultado, o jogador poderá retornar ao menu e iniciar outra investigação.

**Confirmation:**
- [ ] O jogador consegue finalizar uma partida.
- [ ] É possível retornar ao fluxo inicial.
- [ ] Uma nova partida pode ser iniciada sem reiniciar manualmente o programa.
