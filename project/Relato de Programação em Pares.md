# Relato de Programação em Pares 

Durante o desenvolvimento do projeto ORÁCULO, foi realizada uma colaboração entre Rayane Pontes e Everton, envolvendo a implementação e a revisão do sistema de animações do jogo, além de melhorias na configuração do ambiente de desenvolvimento e na portabilidade do projeto.

## 1. Implementação do Sistema de Animações

Inicialmente, Everton desenvolveu o arquivo `animation.c`, responsável pela atualização dos frames das animações e pelo cálculo dos retângulos utilizados para selecionar os sprites correspondentes em uma spritesheet.

A implementação contemplava o controle do tempo de exibição dos frames, os diferentes comportamentos de animação — repetição contínua (*REPEATING*) e execução única (*ONESHOT*) — e a possibilidade de reprodução normal ou reversa.

## 2. Revisão do Código e Sugestão de Melhoria

Após a implementação inicial, Rayane realizou uma revisão do código e sugeriu uma alteração na estrutura da função `AnimationFrame()`, com o objetivo de melhorar a flexibilidade do sistema de animações.

A proposta consistiu em utilizar atributos específicos para as dimensões dos frames, como `frame_width` e `frame_height`, além de considerar a margem esquerda da spritesheet por meio do atributo `margin_left`. Essa abordagem busca oferecer maior controle sobre o posicionamento e as dimensões dos frames.

A alteração também levantou uma discussão sobre o acoplamento entre os componentes do sistema. Sua implementação definitiva ainda está em avaliação, portanto, a proposta é registrada como uma melhoria sugerida, e não como uma mudança já consolidada no projeto.

## 3. Implementação do CMake e Portabilidade

Além da revisão do sistema de animações, Rayane sugeriu a implementação do CMake para melhorar a portabilidade do projeto, facilitando a configuração e a compilação do jogo em diferentes sistemas operacionais.

A sugestão foi implementada, contribuindo para reduzir a dependência de um ambiente específico de desenvolvimento e possibilitando a preparação do projeto para execução em sistemas além do Windows.

## 4. Evidências da Colaboração

As evidências das alterações e do processo de desenvolvimento podem ser consultadas no histórico do repositório GitHub, especialmente na branch `test-ray` e na branch `main`.

Parte do trabalho foi desenvolvida inicialmente na branch `developer`. Posteriormente, Everton integrou à branch `main` a primeira versão funcional do jogo, permitindo acompanhar a evolução do projeto a partir de sua versão principal.

Para verificar a colaboração, podem ser consultados os commits, as alterações nos arquivos e o histórico de desenvolvimento das branches mencionadas.

## 5. Resultados

A colaboração contribuiu para a revisão técnica do sistema de animações, a discussão sobre o acoplamento entre componentes e a melhoria da configuração de compilação do projeto.

As atividades envolveram implementação, análise de código, proposição de melhorias e decisões relacionadas à organização e à portabilidade do software. Dessa forma, o trabalho conjunto auxiliou na evolução técnica do ORÁCULO e na preparação de sua estrutura para futuras implementações.

