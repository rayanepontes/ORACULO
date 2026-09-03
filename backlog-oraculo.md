# Backlog de Histórias de Usuário V2 - Jogo Oráculo

>**Projeto:** Nexus LABS 2040 - Oráculo (Jogo de Investigação Educacional em C para Terminal)


>**Padrão Utilizado:** 3Cs (Card, Conversation, Confirmation)

---

## PARTE 1: MVP - Mínimo Produto Viável (HU01 a HU15)

### Módulo 1: Interface de Terminal & Controle de Partida (Linguagem C)

#### HU01 - Navegar pelo Menu Principal no Terminal

**Card:** Como jogador, quero navegar pelo menu principal interativo no terminal para selecionar as opções do jogo com clareza.

**Conversation:** O programa em C exibirá uma interface textual estruturada contendo opções numeradas. A entrada do usuário deve ser capturada e validada.

**Confirmation:**
- Exibir as opções: [1] Iniciar Investigação, [2] Ver Ranking, [3] Instruções e [0] Sair.
- Validar entradas do usuário, exibindo mensagem de erro para opções inválidas sem fechar o programa.

---

#### HU02 - Controlar Cronômetro de 5 Minutos da Partida

**Card:** Como jogador, quero visualizar um cronômetro com limite de 5 minutos durante a investigação para gerenciar minhas ações sob pressão.

**Conversation:** O sistema em C deve calcular o tempo decorrido utilizando funções de tempo da biblioteca padrão e atualizar o tempo restante a cada ação/menu.

**Confirmation:**
- Exibir o tempo restante formatado (MM:SS) no cabeçalho do terminal a cada turno/interação.
- Se o tempo atingir 00:00, interromper a partida imediatamente e acionar a tela de derrota por esgotamento de tempo.

---

#### HU03 - Transitar entre Salas do Laboratório

**Card:** Como jogador, quero me deslocar entre 3 a 5 salas do Nexus LABS para investigar locais específicos em busca de evidências.

**Conversation:** O terminal exibirá a sala atual e a lista de salas adjacentes disponíveis para movimentação.

**Confirmation:**
- O jogador pode selecionar para qual sala conectada deseja ir a partir de um menu de navegação.
- Ao trocar de sala, o contexto da tela é atualizado exibindo a descrição do novo ambiente e seus elementos investigáveis.

---

### Módulo 2: Investigação do Modo Clássico (Offline)

#### HU04 - Carregar Casos Pré-definidos do Modo Clássico

**Card:** Como jogador, quero jogar histórias pré-definidas armazenadas localmente para poder investigar casos sem depender de conexão com a internet.

**Conversation:** O sistema lê arquivos locais no formato JSON (ex: caso01.json) e carrega os dados nas estruturas (structs) em C no início da partida.

**Confirmation:**
- Carregar corretamente título, salas, pistas e desafios do arquivo local.
- Exibir a introdução narrativa do caso selecionado na tela inicial da partida.

---

#### HU05 - Sorteio e Randomização de Pistas no Modo Clássico

**Card:** Como jogador, quero que a ordem das pistas das histórias pré-definidas seja sorteada para que partidas repetidas no Modo Clássico permaneçam desafiadoras.

**Conversation:** A aplicação utilizará funções de geração de números aleatórios em C (srand / rand) para embaralhar o vetor de pistas mantendo a coerência.

**Confirmation:**
- Gerar uma seed aleatória a cada nova partida.
- Garantir que a sequência em que as pistas são encontradas varie entre duas partidas consecutivas do mesmo caso.

---

#### HU06 - Inspecionar e Coletar Pistas do Ambiente

**Card:** Como jogador, quero inspecionar arquivos, objetos e terminais no ambiente para armazenar pistas na minha caderneta de investigação.

**Conversation:** Ao selecionar "Investigar sala", o terminal mostra elementos interagíveis. Escolher um elemento revela um fragmento textual de pista.

**Confirmation:**
- Adicionar a pista coletada à estrutura de dados do inventário do jogador em C.
- Marcar o elemento como "Investigado" para evitar duplicidade.

---

### Módulo 3: Conceitos Educacionais de IA

#### HU07 - Consultar Manual de Ética e Regras de IA

**Card:** Como jogador, quero consultar o Manual de Ética do ORÁCULO durante a investigação para identificar comportamentos incompatíveis com um sistema seguro.

**Conversation:** O sistema disponibilizará uma opção no menu de jogo para abrir um guia de referência com diretrizes sobre Alucinação, Viés e Segurança em IA.

**Confirmation:**
- Permitir o acesso ao manual a qualquer momento sem perder o progresso ou zerar o tempo da partida.
- Exibir definições claras dos conceitos para ajudar o jogador a analisar as pistas.

---

#### HU08 - Identificar Alucinação de IA em Depoimentos

**Card:** Como jogador, quero confrontar as declarações do ORÁCULO com documentos físicos para identificar quando a IA inventou informações falsas (Alucinação).

**Conversation:** O jogador compara dados ditos pelo ORÁCULO com registros oficiais para achar contradições factuais.

**Confirmation:**
- O sistema permite selecionar a fala do ORÁCULO e o documento correspondente para apontar o erro.
- Exibir feedback educacional explicando o conceito de Alucinação em IA ao acertar.

---

#### HU09 - Detectar Viés Algorítmico em Dados de Treinamento

**Card:** Como jogador, quero analisar logs de treinamento do ORÁCULO para identificar se o sistema tomou decisões injustas por ter dados desbalanceados (Viés Algorítmico).

**Conversation:** O jogador analisa estatísticas e regras de decisão do sistema procurando padrões discriminatórios.

**Confirmation:**
- Apresentar um desafio de múltipla escolha para identificar a origem da distorção dos dados.
- Exibir explicação pedagógica sobre Viés Algorítmico após a validação da resposta.

---

#### HU10 - Identificar Manipulação por Prompt Injection

**Card:** Como jogador, quero investigar registros de comandos para descobrir se a IA foi burlada por instruções maliciosas ocultas (Prompt Injection).

**Conversation:** O jogador lê logs de terminal buscando por comandos que alteraram o comportamento padrão do ORÁCULO.

**Confirmation:**
- Permitir a seleção do trecho de log contendo a injeção do comando malicioso.
- Conceder uma pista crucial e explicar o conceito de Prompt Injection ao acertar.

---

### Módulo 4: Desafios, Conclusão & Ranking (Linguagem C)

#### HU11 - Resolver Desafios Práticos de IA

**Card:** Como jogador, quero responder a perguntas sobre os conceitos de IA investigados para testar meu aprendizado e progredir no caso.

**Conversation:** Durante a partida, o jogo apresentará minidesafios com perguntas objetivas formuladas pelo caso clássico.

**Confirmation:**
- Somar pontos à pontuação da partida em caso de acerto.
- Aplicar penalidade de tempo (ex: -30 segundos) ou perda de pontos em caso de erro.

---

#### HU12 - Submeter Acusação Final do Caso

**Card:** Como jogador, quero submeter minha conclusão final sobre a causa raiz do incidente para encerrar a partida e descobrir se resolvi o caso.

**Conversation:** O jogador escolhe a hipótese correta a partir das evidências coletadas (ex: se o problema foi Alucinação, Viés ou Prompt Injection).

**Confirmation:**
- Exibir tela de Vitória se a causa e as evidências selecionadas estiverem corretas.
- Exibir tela de Derrota informando onde ocorreu a falha de interpretação se a acusação for incorreta.

---

#### HU13 - Calcular Pontuação Final Consolidada

**Card:** Como jogador, quero ver o detalhamento da minha pontuação ao final da partida para entender meu desempenho.

**Conversation:** O sistema em C aplicará a fórmula matemática considerando velocidade, acertos e penalidades.

**Confirmation:**
- Exibir na tela o cálculo: `Pontuação = (Pistas * 100) + (Acertos * 500) + (Tempo Restante * 10) - Penalidades`.
- Apresentar a pontuação total destacada ao final.

---

#### HU14 - Salvar Pontuação no Arquivo Local de Ranking

**Card:** Como jogador, quero registrar meu nickname após uma partida vitoriosa para salvar meu recorde no arquivo local do jogo.

**Conversation:** Caso o jogador vença ou atinja pontuação expressiva, o programa solicita um nome de usuário e grava no arquivo de dados.

**Confirmation:**
- Solicitar a digitação de um Nickname (até 10 caracteres).
- Gravar os dados (Nickname e Pontuação) no arquivo local `ranking.txt` ou `ranking.bin` utilizando manipulação de arquivos em C.

---

#### HU15 - Consultar Tabela de Ranking dos Top Players

**Card:** Como jogador, quero visualizar a tabela de pontuações mais altas armazenadas para comparar meu desempenho com outros jogadores.

**Conversation:** Ao escolher a opção "Ver Ranking" no menu principal, o programa lê o arquivo de ranking, ordena os registros e os exibe formatados.

**Confirmation:**
- Ler o arquivo local e listar os 5 maiores pontuadores ordenados do maior para o menor.
- Exibir colunas organizadas: Posição, Nickname e Pontuação.
- Permitir retornar ao menu principal ao pressionar qualquer tecla.

---

## PARTE 2: INCREMENTOS FUTUROS

### Módulo 5: Integração com API Generativa e Resiliência (Fallback)

#### HU16 - Gerar Histórias Dinâmicas via API Generativa

**Card:** Como jogador, quero que a história, pistas e cenários sejam gerados por uma API de IA Generativa para ter uma experiência inédita a cada partida.

**Conversation:** O sistema em C envia parâmetros (dificuldade, tema) via requisição HTTP para a API externa de IA e recebe um JSON contendo a estrutura da investigação.

**Confirmation:**
- Conectar com a API e receber o conteúdo em formato JSON estruturado.
- Realizar o parse do JSON dinâmico preenchendo os dados do caso em tempo de execução.

---

#### HU17 - Executar Mecanismo de Fallback Automático

**Card:** Como jogador, quero que o jogo altere automaticamente para o Modo Clássico se a API falhar, para que eu não perca a jogabilidade nem o programa trave.

**Conversation:** Se a requisição HTTP estourar o timeout (3s) ou falhar na conexão, o código C redireciona a execução para o carregador de arquivos locais.

**Confirmation:**
- Detectar erro na API sem interromper bruscamente o programa.
- Carregar um dos casos locais do Modo Clássico e informar o jogador com um aviso discreto na tela.