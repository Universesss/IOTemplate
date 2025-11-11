import json
import os

# JSON dosyasının yolu
json_path = os.path.join(".vscode", "vex_project_settings.json")
config_path = os.path.join("include", "config.h")

# JSON oku
with open(json_path) as f:
    data = json.load(f)

slot = data.get("slot", 1)  # Eğer yoksa varsayılan 1

# config.h dosyasını oluştur
with open(config_path, "w") as f:
    f.write("#pragma once\n")
    f.write(f"#define AUTON_SLOT {slot}\n")

print(f"[CONFIG] AUTON_SLOT set to {slot}")
