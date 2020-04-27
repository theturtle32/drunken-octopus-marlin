#!/bin/sh

####
# Portions (C) 2020, Marcio Teixeira
# Portions (C) 2019, AlephObjects, Inc.
#
#
# The bash script in this page is free software: you can
# redistribute it and/or modify it under the terms of the GNU Affero
# General Public License (GNU AGPL) as published by the Free Software
# Foundation, either version 3 of the License, or (at your option)
# any later version.  The code is distributed WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU AGPL for more details.
#

fetch_default_config() {
    CONF_URL=https://raw.githubusercontent.com/MarlinFirmware/Configurations/bugfix-2.0.x/config/default
    CONF_DIR=../../default
    (wget $CONF_URL/Configuration.h -O $CONF_DIR/Configuration.h &&
    wget $CONF_URL/Configuration_adv.h -O $CONF_DIR/Configuration_adv.h &&
    cat  $CONF_DIR/Configuration_DrunkenOctopus.h >> $CONF_DIR/Configuration.h) ||
    exit 1
}

build_config() {
  group=$1
  printer_name=$2
  toolhead_name=$3

  echo ${group}/${printer_name}/${toolhead_name}
  ./build-config.py $printer_name $toolhead_name -D ${group}/${printer_name}/${toolhead_name} --summary
}

if [ "$1" = "upgrade" ]; then
  fetch_default_config || (echo Unable to retrieve new configuration files; exit 1)
else
  echo
  echo
  echo Using pre-existing config files. To update, use "./build-configs.sh upgrade"
  echo
  echo
fi

build_config standard  Gladiola_Mini                             Gladiola_SingleExtruder
build_config accessory Gladiola_Mini                             Finch_Aerostruder
build_config accessory Gladiola_Mini                             CecropiaSilk_SingleExtruderAeroV2
build_config accessory Gladiola_Mini                             Albatross_Flexystruder
build_config accessory Gladiola_Mini                             AchemonSphinx_SmallLayer
build_config accessory Gladiola_Mini                             BandedTiger_HardenedSteel
build_config accessory Gladiola_Mini                             DingyCutworm_HardenedSteelPlus
build_config accessory Gladiola_Mini                             Goldenrod_HardenedExtruder

build_config custom    Gladiola_MiniLCD                          Gladiola_SingleExtruder
build_config custom    Gladiola_MiniLCD                          Finch_Aerostruder
build_config custom    Gladiola_MiniLCD                          CecropiaSilk_SingleExtruderAeroV2
build_config custom    Gladiola_MiniLCD                          Albatross_Flexystruder
build_config custom    Gladiola_MiniLCD                          AchemonSphinx_SmallLayer
build_config custom    Gladiola_MiniLCD                          BandedTiger_HardenedSteel
build_config custom    Gladiola_MiniLCD                          DingyCutworm_HardenedSteelPlus
build_config custom    Gladiola_MiniLCD                          Goldenrod_HardenedExtruder
build_config custom    Gladiola_MiniTouchUSB                     Gladiola_SingleExtruder
build_config custom    Gladiola_MiniEinsyLCD                     Gladiola_SingleExtruder
build_config custom    Gladiola_MiniEinsyLCD                     CecropiaSilk_SingleExtruderAeroV2
build_config custom    Gladiola_MiniEinsyLCD                     BandedTiger_HardenedSteel
build_config custom    Gladiola_MiniEinsyTouchUSB                Gladiola_SingleExtruder

build_config accessory Hibiscus_Mini2                            AchemonSphinx_SmallLayer
build_config standard  Hibiscus_Mini2                            CecropiaSilk_SingleExtruderAeroV2
build_config accessory Hibiscus_Mini2                            BandedTiger_HardenedSteel
build_config accessory Hibiscus_Mini2                            DingyCutworm_HardenedSteelPlus
build_config accessory Hibiscus_Mini2                            Goldenrod_HardenedExtruder
build_config custom    Hibiscus_Mini2TouchSD                     CecropiaSilk_SingleExtruderAeroV2
build_config custom    Hibiscus_Mini2TouchUSB                    CecropiaSilk_SingleExtruderAeroV2

build_config standard  Juniper_TAZ5                              Tilapia_SingleExtruder
build_config accessory Juniper_TAZ5                              Angelfish_Aerostruder
build_config accessory Juniper_TAZ5                              CecropiaSilk_SingleExtruderAeroV2
build_config accessory Juniper_TAZ5                              Kanyu_Flexystruder
build_config accessory Juniper_TAZ5                              Opah_Moarstruder
build_config accessory Juniper_TAZ5                              Javelin_DualExtruderV2
build_config accessory Juniper_TAZ5                              Longfin_FlexyDually
build_config accessory Juniper_TAZ5                              Yellowfin_DualExtruderV3
build_config accessory Juniper_TAZ5                              AchemonSphinx_SmallLayer
build_config accessory Juniper_TAZ5                              BandedTiger_HardenedSteel
build_config accessory Juniper_TAZ5                              DingyCutworm_HardenedSteelPlus
build_config accessory Juniper_TAZ5                              Goldenrod_HardenedExtruder
build_config custom    Juniper_TAZ5Archim                        Tilapia_SingleExtruder
build_config custom    Juniper_TAZ5BLTouch                       Tilapia_SingleExtruder
build_config custom    Juniper_TAZ5ArchimBLTouch                 Tilapia_SingleExtruder
build_config custom    Juniper_TAZ5Archim                        Angelfish_Aerostruder
build_config custom    Juniper_TAZ5BLTouch                       Angelfish_Aerostruder
build_config custom    Juniper_TAZ5ArchimBLTouch                 Angelfish_Aerostruder

build_config standard  Oliveoil_TAZ6                             Tilapia_SingleExtruder
build_config accessory Oliveoil_TAZ6                             Angelfish_Aerostruder
build_config accessory Oliveoil_TAZ6                             CecropiaSilk_SingleExtruderAeroV2
build_config accessory Oliveoil_TAZ6                             Kanyu_Flexystruder
build_config accessory Oliveoil_TAZ6                             Opah_Moarstruder
build_config accessory Oliveoil_TAZ6                             Javelin_DualExtruderV2
build_config accessory Oliveoil_TAZ6                             Longfin_FlexyDually
build_config accessory Oliveoil_TAZ6                             Yellowfin_DualExtruderV3
build_config accessory Oliveoil_TAZ6                             AchemonSphinx_SmallLayer
build_config accessory Oliveoil_TAZ6                             BandedTiger_HardenedSteel
build_config accessory Oliveoil_TAZ6                             DingyCutworm_HardenedSteelPlus
build_config accessory Oliveoil_TAZ6                             Goldenrod_HardenedExtruder
build_config custom    Oliveoil_TAZ6Archim                       Opah_Moarstruder
build_config custom    Oliveoil_TAZ6Archim                       Tilapia_SingleExtruder
build_config custom    Oliveoil_TAZ6Archim                       AchemonSphinx_SmallLayer
build_config custom    Oliveoil_TAZ6BLTouch                      Tilapia_SingleExtruder
build_config custom    Oliveoil_TAZ6BLTouch                      Yellowfin_DualExtruderV3
build_config custom    Oliveoil_TAZ6BLTouchHallEffect            Yellowfin_DualExtruderV3
build_config custom    Oliveoil_TAZ6BLTouch                      CecropiaSilk_SingleExtruderAeroV2
build_config custom    Oliveoil_TAZ6ArchimBLTouch                Tilapia_SingleExtruder
build_config custom    Oliveoil_TAZ6Archim                       Angelfish_Aerostruder
build_config custom    Oliveoil_TAZ6BLTouch                      Angelfish_Aerostruder
build_config custom    Oliveoil_TAZ6ArchimBLTouch                Angelfish_Aerostruder
build_config custom    Oliveoil_TAZ6Archim                       E3D_Hermera
build_config custom    Oliveoil_TAZ6BLTouch                      E3D_Hermera
build_config custom    Oliveoil_TAZ6ArchimBLTouch                E3D_Hermera

build_config accessory Quiver_TAZPro                             AchemonSphinx_SmallLayer
build_config accessory Quiver_TAZPro                             CecropiaSilk_SingleExtruderAeroV2
build_config accessory Quiver_TAZPro                             BandedTiger_HardenedSteel
build_config accessory Quiver_TAZPro                             DingyCutworm_HardenedSteelPlus
build_config accessory Quiver_TAZPro                             Goldenrod_HardenedExtruder
build_config standard  Quiver_TAZPro                             Quiver_DualExtruder

build_config standard  Redgum_TAZWorkhorse                       Goldenrod_HardenedExtruder
build_config accessory Redgum_TAZWorkhorse                       CecropiaSilk_SingleExtruderAeroV2
build_config accessory Redgum_TAZWorkhorse                       AchemonSphinx_SmallLayer
build_config accessory Redgum_TAZWorkhorse                       BandedTiger_HardenedSteel
build_config accessory Redgum_TAZWorkhorse                       DingyCutworm_HardenedSteelPlus
build_config accessory Redgum_TAZWorkhorse                       Yellowfin_DualExtruderV3
build_config accessory Redgum_TAZWorkhorse                       E3D_Hermera
build_config custom    Redgum_TAZWorkhorseArchim                 Goldenrod_HardenedExtruder
build_config custom    Redgum_TAZWorkhorseBLTouch                Goldenrod_HardenedExtruder
build_config custom    Redgum_TAZWorkhorseArchimBLTouch          Goldenrod_HardenedExtruder
build_config custom    Redgum_TAZWorkhorseArchim                 E3D_Hermera
build_config custom    Redgum_TAZWorkhorseBLTouch                E3D_Hermera
build_config custom    Redgum_TAZWorkhorseArchimBLTouch          E3D_Hermera
build_config custom    Redgum_TAZWorkhorseArchimTouchUSBBLTouch  E3D_Hermera

build_config standard  KangarooPaw_Bio                           KangarooPaw_SingleExtruder

build_config custom    Experimental_TouchDemo                    Gladiola_SingleExtruder
build_config custom    Experimental_BLTouch                      E3D_Hermera