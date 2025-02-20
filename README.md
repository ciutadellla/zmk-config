
# Docker ZMK dev env ciutad

[Docks](https://zmk.dev/docs/development/local-toolchain/setup/container#creating-volumes)


```shell

git clone https://github.com/zmkfirmware/zmk.git

docker volume create --driver local -o o=bind -o type=none \
  -o device="/Users/ciutadellla/playgroud/zmk-config/" zmk-config

docker volume create --driver local -o o=bind -o type=none \
  -o device="/Users/ciutadellla/playgroud/zmk-config/modules/" zmk-modules


devcontainer up --workspace-folder "/Users/ciutadellla/playgroud/dockerzmk/zmk"


```



```shell
docker exec -w /workspaces/zmk -it 51bc9efbb6b9 /bin/bash

west init -l app/
west update  
```

```shell
cd app
```

```shell
west build -p -b nice_nano_v2 -- -DSHIELD="reviung41 nice_view_adapter nice_view" -DZMK_CONFIG="/workspaces/zmk-config/config" -DZMK_EXTRA_MODULES="/workspaces/zmk-config/modules/zmk-helpers"

```