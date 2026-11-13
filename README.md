# Play! PS2 Emulator for PS3 - Port v2.4

Port do emulador Play! (PS2) para PlayStation 3 usando PSL1GHT.

## Estrutura do Projeto

```
play-ps3-port/
├── Makefile              # Build system (PSL1GHT)
├── package.conf          # Metadados do pacote NPDRM
├── Source/
│   └── ui_ps3/
│       └── main_ps3.cpp  # Ponto de entrada PS3
└── .github/
    └── workflows/
        └── build.yml     # GitHub Actions workflow
```

## Build Local (requer PSL1GHT)

```bash
# Configurar variável de ambiente
export PSL1GHT=/opt/psl1ght

# Compilar
make pkg

# O arquivo .pkg será gerado: play_ps3.pkg
```

## Build Automático (GitHub Actions)

1. **Crie um repositório no GitHub** e faça push destes arquivos:
   ```bash
   git init
   git add .
   git commit -m "Initial commit: Play! PS3 port v2.4"
   git branch -M main
   git remote add origin https://github.com/SEU_USUARIO/play-ps3-port.git
   git push -u origin main
   ```

2. **O build inicia automaticamente** ao fazer push para `main` ou `master`.

3. **Baixe o .pkg** em:
   - Vá na aba **Actions** do repositório
   - Clique no workflow "Build Play! PS3 PKG" concluído
   - Em **Artifacts**, baixe `play_ps3_pkg`

## Metadados do Pacote (package.conf)

- **ContentID**: UP0001-PLAYPS300_00-0000000000000001
- **Título**: Play! PS2 Emulator v2.3.1
- **AppID**: PLAYPS300
- **DRM**: Free (sem proteção)
- **Tipo**: GameExec

## Requisitos no PS3

- PS3 com CFW (Custom Firmware) ou HEN
- BIOS do PS2 em `/dev_hdd0/game/PLAYPS300/USRDIR/bios/`
- ISOs de jogos PS2 em `/dev_hdd0/PS2ISO/` ou USB em `/dev_usbXXX/PS2ISO/`

## ⚠️ Avisos Importantes

- **Este repositório NÃO inclui BIOS nem jogos** - você deve fornecer os seus próprios arquivos que tenha direito de usar
- O emulador Play! original é necessário para compilação completa (headers como `PS2VM.h`, `AppConfig.h`, etc.)
- Este port contém apenas o código de integração PS3 (main_ps3.cpp)

## Licença

Baseado no emulador Play! - verifique a licença do projeto original.