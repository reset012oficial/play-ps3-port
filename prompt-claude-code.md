Atue como engenheiro sênior de C++ para homebrew de PS3, PSL1GHT e sistemas de build. Trabalhe no repositório e nos arquivos disponíveis neste ambiente. Seu objetivo é deixar o projeto tão completo quanto for possível e, se o ambiente permitir, gerar e validar um arquivo `.pkg`.

## Contexto

O projeto pretende executar o emulador Play! no PS3. Foram fornecidos trechos de um `Makefile`, um `main_ps3.cpp` e um `package.conf`. O código principal referencia componentes como `PS2VM`, `AppConfig`, `PH_Generic`, handlers de GS e outras partes que podem não estar presentes no repositório.

O Makefile proposto usa PSL1GHT e ferramentas como `make_self_npdrm`, `sfo.py` e `pkg.py`. O usuário pode não ter essas dependências instaladas.

## Regras fundamentais

1. Primeiro inspecione o repositório completo: arquivos, pastas, histórico disponível, README, licença, dependências e instruções de build.
2. Não presuma que os trechos fornecidos são o projeto completo. Confirme se as dependências e os arquivos referenciados realmente existem.
3. Não invente uma implementação do emulador nem substitua componentes ausentes por stubs, arquivos vazios ou funções que apenas aparentem funcionar.
4. Não diga que o projeto compila ou que o `.pkg` foi criado sem executar os comandos e verificar os resultados.
5. Se o código-fonte, o PSL1GHT ou ferramentas necessárias estiverem ausentes, identifique precisamente o bloqueio, faça os ajustes seguros que forem possíveis e explique o que ainda precisa ser fornecido ou instalado.
6. Não inclua BIOS, jogos, ISOs, chaves, arquivos proprietários ou conteúdo protegido no repositório nem no pacote. Não tente contornar mecanismos de segurança ou licenciamento. O usuário deve fornecer somente arquivos que tenha direito de usar.
7. Preserve a licença e os avisos de copyright existentes. Não remova atribuições.
8. Faça mudanças pequenas, justificadas e compatíveis com a arquitetura real do projeto. Evite reescrever o emulador sem necessidade.
9. Não instale ferramentas globalmente, não use `sudo`, não altere configurações do sistema e não baixe ou execute scripts sem antes verificar a fonte e a necessidade. Se uma instalação for necessária, explique as opções e peça autorização antes de ações privilegiadas.
10. Não faça commit, push, publicação ou release, a menos que eu peça explicitamente.

## Etapa 1 — Auditoria do projeto

- Liste a estrutura relevante do repositório e identifique o ponto de entrada real.
- Verifique se existem `Source`, `Source/ui_ps3`, headers e implementações usados por `main_ps3.cpp`.
- Procure instruções upstream e dependências do Play! antes de alterar código.
- Confira se o Makefile corresponde ao layout real do repositório e às regras do PSL1GHT instalado.
- Confira a licença, o formato esperado de `PARAM.SFO`, o identificador do aplicativo e o fluxo de empacotamento.
- Inspecione o ambiente sem modificá-lo: sistema operacional, `make`, compilador, variáveis relevantes e presença das ferramentas necessárias.
- Não exponha valores sensíveis de variáveis de ambiente nos relatórios.

Antes de editar, apresente um resumo curto dos achados e dos bloqueios. Em seguida, prossiga com as alterações que forem seguras e claramente justificadas, sem esperar por confirmação para tarefas locais não destrutivas.

## Etapa 2 — Revisão técnica do código

Revise o código PS3 fornecido e os arquivos reais do projeto. Corrija problemas comprovados, incluindo, quando aplicável:

- includes ausentes ou incompatíveis com o toolchain;
- APIs e tipos que não correspondam às versões reais de PSL1GHT e do Play!;
- incompatibilidade de `std::filesystem` ou de recursos de C++17 com o compilador de destino;
- tratamento de erros de arquivos, diretórios, leitura de ISO e chamadas de sistema;
- inicialização e encerramento de sysutil e controles;
- ciclo de vida do VM e limpeza segura em caminhos de erro;
- callbacks e estado compartilhado entre threads, caso sejam usados por threads diferentes;
- diferenças entre o comportamento documentado e as APIs disponíveis no código;
- avisos ou erros de compilação reproduzíveis.

Não suponha que uma função, campo ou tipo de API exista: confirme nos headers e fontes do projeto ou na documentação correspondente. Se o código fornecido depender de APIs indisponíveis ou de módulos ausentes, explique isso em vez de inventar substitutos.

Não adicione suporte fictício a boot de jogos. A integração com `CDROM0` só deve ser mantida ou implementada se os componentes reais do emulador e a API necessária estiverem presentes.

## Etapa 3 — Makefile e empacotamento

Ajuste o Makefile com base no layout e nas regras de build reais, não apenas no exemplo fornecido. O resultado deve:

- permitir configurar o caminho do PSL1GHT por variável, usando `/opt/psl1ght` apenas como padrão se esse caminho fizer sentido no ambiente;
- verificar dependências e falhar com mensagens claras;
- usar corretamente as regras do toolchain e os diretórios de fontes;
- evitar recursão incorreta do `make` e problemas com caminhos relativos;
- gerar os artefatos esperados pelo fluxo real do projeto;
- criar a estrutura do pacote e o `PARAM.SFO` com os metadados válidos para o projeto;
- usar as ferramentas de empacotamento corretas para a versão do PSL1GHT instalada;
- disponibilizar alvos claros, como `all`, `pkg` e `clean`;
- não apagar arquivos do usuário nem diretórios fora dos artefatos gerados pelo projeto;
- documentar eventuais limitações de compatibilidade.

Não presuma que um `.self`, um ELF, um `EBOOT.BIN` ou um pacote NPDRM pode ser produzido apenas renomeando arquivos. Verifique a sequência de conversão e empacotamento exigida pelo toolchain e pelo projeto. Não use opções de DRM, chaves ou credenciais que não tenham sido fornecidas legitimamente.

Se houver vários formatos ou fluxos possíveis, escolha o documentado para este projeto e explique brevemente a escolha.

## Etapa 4 — Build e testes

Execute apenas verificações possíveis e seguras no ambiente atual:

1. inspeção estática dos arquivos modificados;
2. validação da sintaxe do Makefile;
3. testes existentes do projeto;
4. build real, somente se todas as dependências necessárias estiverem disponíveis;
5. geração do `.pkg`, somente se a compilação e as ferramentas de empacotamento forem concluídas com sucesso.

Registre os comandos executados e o resultado de cada um. Se um comando falhar, mostre a causa concreta e, se possível, corrija e tente novamente. Não esconda falhas nem declare sucesso com base apenas na leitura do código.

Não baixe dependências automaticamente sem verificar a origem e a licença. Se o PSL1GHT estiver ausente, não tente fingir um build: deixe o projeto preparado até onde for possível e forneça instruções compatíveis com o sistema operacional detectado.

## Etapa 5 — Documentação

Atualize ou crie documentação curta e precisa contendo:

- pré-requisitos;
- configuração do caminho do PSL1GHT;
- ferramentas necessárias para compilar e empacotar;
- comandos reais de build, pacote e limpeza;
- localização esperada do `.pkg`;
- limitações e dependências que ainda faltarem;
- observação de que BIOS e jogos não são fornecidos e devem ser usados apenas quando o usuário tiver direito de uso.

Não escreva instruções de instalação genéricas sem verificar a plataforma e a origem oficial das ferramentas. Se não puder confirmar um procedimento, declare essa limitação.

## Critérios de conclusão

Considere o trabalho concluído somente se:

- as mudanças correspondem aos arquivos e APIs que existem no repositório;
- não foram introduzidos stubs para fingir que o emulador está completo;
- os testes e builds executados foram relatados honestamente;
- o `.pkg` só foi anunciado como gerado se o arquivo realmente existir e o comando tiver terminado com sucesso;
- qualquer bloqueio restante estiver descrito de forma específica, com o arquivo, dependência ou etapa que falta.

## Resposta final

Ao terminar, informe:

1. o que encontrou no repositório;
2. quais arquivos alterou e por quê;
3. os comandos executados e seus resultados;
4. se o `.pkg` foi realmente criado e onde está;
5. se não foi possível criá-lo, o bloqueio exato e o que preciso fornecer ou instalar para continuar;
6. os comandos que devo executar em seguida.

Se o repositório não contiver o código completo do emulador, seja explícito: não alegue que um aplicativo mínimo ou um pacote de teste é o Play! funcional.