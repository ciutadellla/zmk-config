
# Docker ZMK dev env ciutad

[Docks](https://zmk.dev/docs/development/local-toolchain/setup/container#creating-volumes)
[ZMK-HELPER](https://github.com/urob/zmk-helpers)

```shell

git clone https://github.com/zmkfirmware/zmk.git

docker volume create --driver local -o o=bind -o type=none \
  -o device="/Users/ciutadellla/playgroud/zmk-config/" zmk-config

docker volume create --driver local -o o=bind -o type=none \
  -o device="/Users/ciutadellla/playgroud/zmk-config/modules/" zmk-modules

devcontainer up --workspace-folder "/Users/ciutadellla/playgroud/dockerzmk/zmk"


```

```shell
docker exec -w /workspaces/zmk -it 3fdfbdf50178 /bin/bash

west init -l app/
west update  
```

# fssnive_view and nice_view_bongo has 2 displaywidgets costum or normal

```shell
cd app

west build -p -b nice_nano_v2 -- -DSHIELD="reviung41 nice_view_adapter nice_view" -DZMK_CONFIG="/workspaces/zmk-config/config" -DZMK_EXTRA_MODULES="/workspaces/zmk-config/config/zmk-helpers"


west build -p -b nice_nano_v2 -- -DSHIELD="reviung41 nice_view_adapter nice_view_bongo" -DZMK_CONFIG="/workspaces/zmk-config/config" -DZMK_EXTRA_MODULES="/workspaces/zmk-config/config/zmk-helpers"

```

