# Sistema de Búsqueda de Reseñas de Anime del año 2018

Este documento describe el sistema de búsqueda de Anime con datos del año 2018, incluyendo la descripción del dataset utilizado, los campos clave, los criterios de búsqueda implementados, la justificación de estos criterios y ejemplos de uso del programa.

## 1. Descripción de la Base de Datos

Este conjunto de datos contiene valoraciones de usuarios e información sobre diversos títulos de anime. Está diseñado para crear un sistema de recomendaciones de anime (basado en datos de 2018). Su descarga gratuita está disponible al público en: https://www.kaggle.com/datasets/dbdmobile/myanimelist-dataset?select=final_animedataset.csv el dataset usado en este caso es final_animedataset.csv

## 2. Campos del Dataset Seleccionado

A continuación, se presenta una descripción detallada de los campos del dataset proporcionado, incluyendo los rangos de entrada válidos para cada uno.

- **username**

  - Descripción: El nombre de usuario del usuario que valoró el anime.
  - Tipo: Cadena de texto.
  - Ejemplo: karthiga.

- **anime_id**

  - Descripción: El identificador único del anime.
  - Tipo: Número entero.
  - Ejemplo: 1557.

- **my_score**

  - Descripción: La puntuación otorgada por el usuario al anime.
  - Tipo: Número entero de 0-10.
  - Ejemplo: 8.

- **user_id**

  - Descripción: El identificador único del usuario.
  - Tipo: número entero.
  - Ejemplo: 1897606.

- **gender**

  - Descripción: Genero del usuario.
  - Tipo: Cadena de texto (Male, Female).
  - Ejemplo: Male.

- **title**

  - Descripción: El titulo del anime.
  - Tipo: Cadena de texto.
  - Ejemplo: Kiniro no Corda: Primo Passo.

- **type**

  - Descripción: El tipo de anime (por ejemplo, serie de televisión, película, OVA, etc.).
  - Tipo: Cadena de texto.
  - Ejemplo: TV.

- **source**

  - Descripción: El material original del anime (por ejemplo, manga, novela ligera, original, etc.).
  - Tipo: Cadena de texto.
  - Ejemplo: Manga.

- **score**

  - Descripción: La puntuación general del anime.
  - Tipo: Número de punto flotante.
  - Ejemplo: 7.77.

- **scored_by**

  - Descripción: El número de usuarios que han calificado el anime.
  - Tipo: Número entero.
  - Ejemplo: 167968.

## 3. Criterios de Búsqueda Implementados

Los criterios de búsqueda implementados son los siguientes:

- Nombre de usuario(username): Busca todas las calificaciones de ese usuario.
- Nombre del anime(title): Busca todas las calificaciones que tenga el usuario de ese anime.
- Tipo de anime.

### Justificación de los Criterios

El **Nombre de usuario** es un campo que permite flexibilidad en el uso de la interfaz, ya que permite exploraciones amplias de todas las calificaciones de un usario.

El **Nombre del anime** es un campo de busqueda primario que necesita del nombre de usuario, se usa para busquedas específicas, permite exploraciones precisas de la calificación de ese anime por el usuario ingresado.

El **Tipo de anime** se designa como otro campo de búsqueda primario. Su función es actuar como un filtro que permite exploraciones de todas las calificaciones de ese tipo de anime de un usuario.

### Rangos de Valores Válidos para las Entradas

- **Nombre de usuario**: Cadena de caracteres de hasta 256 caracteres de longitud.
- **Nombre del anime**: Cadena de caracteres de hasta 256 caracteres de longitud.
- **Tipo de anime**: Cadena de texto de 5 a 12 caracteres de longitud.

## 4. Ejemplos de Uso del Programa

### 4.1. Librerias Necesarias

Para compilar el proyecto, necesitas la libreria GTK + las herramientas de compilación, además del CSV necesario, puedes instalarlo así:

- Debian / Ubuntu / derivados:

```bash
sudo apt update
sudo apt install build-essential pkg-config libgtk-3-dev
```

- Fedora:

```bash
sudo dnf install @development-tools pkg-config gtk3-devel
```

- Arch Linux / Manjaro:

```bash
sudo pacman -Syu base-devel pkg-config gtk3
```

Los paquetes clave son pkg-config y libgtk-3-dev (o gtk3-devel) para compilar aplicaciones GTK3 con gcc.

### 4.2. Pasos de Ejecución

Para compilar cada componente de tu programa, usa los siguientes comandos:

```bash
gcc indexer.c hashmap.c -o indexer
gcc back.c hashmap.c -o back
gcc `pkg-config --cflags gtk+-3.0` gui.c -o gui `pkg-config --libs gtk+-3.0`
```

Una vez compilado, el primer paso es generar el archivo índice de los hashes de todos los archivos CSV. Para hacer esto, ejecuta:

```bash
./indexer
```

Después de generar el índice, necesitas ejecutar el backend en segundo plano:

```bash
./back &
```

Por último ejecuta el frontend:

```bash
./gui
```

## 4.3. Ejecución con Makefile

Para compilar y ejecutar el proyecto con el makefile, ejecuta:

```bash
make all
```

Si solo quieres ejecutar el backend y el frontend, ejecuta:

```bash
make run
```

Si solo quieres compilar el proyecto, ejecuta:

```bash
make build
```

Si quieres limpiar el proyecto, ejecuta:

```bash
make clean
```

### 4.4. Ejemplos específicos de búsquedas

#### Búsqueda por Nombre de usuario

Este tipo de búsqueda permite al usuario encontrar todas las calificaciones de un usario en espécifico.

Output obtenido:

```bash
Enviando: karthiga,-,-
Comando recibido: karthiga,-,-
Buscando registros para 'karthiga' que coincida con '-','-'...

karthiga,14713,10,2255153,Female,Kamisama Hajimemashita,TV,Manga,8.14,138954,383.0,259,"Comedy, Demons, Supernatural, Romance, Fantasy, Shoujo"
karthiga,14397,7,2255153,Female,Chihayafuru 2,TV,Manga,8.47,62720,116.0,757,"Drama, Game, Josei, School, Slice of Life, Sports"
karthiga,14227,8,2255153,Female,Tonari no Kaibutsu-kun,TV,Manga,7.71,281448,1093.0,88,"Slice of Life, Comedy, Romance, School, Shoujo"
karthiga,12365,8,2255153,Female,Bakuman. 3rd Season,TV,Manga,8.67,107767,50.0,426,"Comedy, Drama, Romance, Shounen"
karthiga,11759,5,2255153,Female,Accel World,TV,Light novel,7.53,236083,1545.0,103,"Action, Game, Sci-Fi, Romance, School"
karthiga,11123,7,2255153,Female,Sekaiichi Hatsukoi 2,TV,Manga,8.23,53285,288.0,945,"Comedy, Drama, Romance, Shounen Ai"
karthiga,11013,8,2255153,Female,Inu x Boku SS,TV,Manga,7.63,139250,1274.0,231,"Comedy, Supernatural, Romance, Shounen"
karthiga,10800,7,2255153,Female,Chihayafuru,TV,Manga,8.29,89081,246.0,348,"Drama, Game, Josei, School, Slice of Life, Sports"
karthiga,10030,8,2255153,Female,Bakuman. 2nd Season,TV,Manga,8.49,122090,108.0,374,"Comedy, Drama, Romance, Shounen"
karthiga,9926,7,2255153,Female,Sekaiichi Hatsukoi,TV,Manga,8.07,69250,469.0,701,"Comedy, Drama, Romance, Shounen Ai"
karthiga,9863,7,2255153,Female,SKET Dance,TV,Manga,8.33,44132,213.0,646,"Comedy, School, Shounen"
karthiga,9656,8,2255153,Female,Kimi ni Todoke 2nd Season,TV,Manga,8.12,133548,412.0,322,"Slice of Life, Drama, Romance, School, Shoujo"
karthiga,9513,7,2255153,Female,Beelzebub,TV,Manga,8.02,136105,528.0,208,"Action, Comedy, Demons, Supernatural, School, Shounen"
karthiga,7817,5,2255153,Female,B-gata H-kei,TV,4-koma manga,7.1,142827,3193.0,285,"Comedy, Romance, Ecchi, School, Seinen"
karthiga,7674,7,2255153,Female,Bakuman.,TV,Manga,8.32,167429,216.0,172,"Comedy, Drama, Romance, Shounen"
karthiga,7054,9,2255153,Female,Kaichou wa Maid-sama!,TV,Manga,8.2,315130,314.0,71,"Comedy, Romance, School, Shoujo"
karthiga,6645,5,2255153,Female,Chu-Bra!!,TV,Manga,6.3,16688,6380.0,1858,"Ecchi, Comedy, School, Seinen"
karthiga,6586,6,2255153,Female,Yume-iro Pâtissière,TV,Manga,8.03,21618,526.0,1630,"Kids, School, Shoujo"
karthiga,6045,6,2255153,Female,Kimi ni Todoke,TV,Manga,8.13,212710,400.0,111,"Slice of Life, Drama, Romance, School, Shoujo"
karthiga,5835,7,2255153,Female,Hanasakeru Seishounen,TV,Manga,7.87,16772,753.0,1654,"Drama, Romance, Shoujo"
karthiga,5690,8,2255153,Female,Nodame Cantabile Finale,TV,Manga,8.34,47983,203.0,995,"Comedy, Josei, Music, Romance"
karthiga,5262,7,2255153,Female,Shugo Chara!! Doki,TV,Manga,7.55,37129,1508.0,1173,"Comedy, Magic, School, Shoujo"
karthiga,4814,7,2255153,Female,Junjou Romantica 2,TV,Manga,8.15,66565,374.0,799,"Comedy, Drama, Romance, Shounen Ai"
karthiga,4722,8,2255153,Female,Skip Beat!,TV,Manga,8.24,90699,282.0,488,"Comedy, Drama, Romance, Shoujo"
karthiga,4477,8,2255153,Female,Nodame Cantabile: Paris-hen,TV,Manga,8.24,53908,281.0,988,"Music, Slice of Life, Comedy, Romance, Josei"
karthiga,4224,6,2255153,Female,Toradora!,TV,Light novel,8.39,557898,174.0,13,"Slice of Life, Comedy, Romance, School"
karthiga,3731,8,2255153,Female,Itazura na Kiss,TV,Manga,7.7,91187,1108.0,473,"Comedy, Romance, Shoujo"
karthiga,3092,8,2255153,Female,Junjou Romantica,TV,Manga,7.99,86423,568.0,568,"Comedy, Drama, Romance, Shounen Ai"
karthiga,2787,8,2255153,Female,Shakugan no Shana II (Second),TV,Light novel,7.72,126085,1066.0,332,"Action, Drama, Fantasy, Romance, School, Supernatural"
karthiga,2564,5,2255153,Female,Code-E,TV,Original,6.49,5866,5635.0,3214,"Comedy, Romance, School, Sci-Fi, Slice of Life"
karthiga,2104,7,2255153,Female,Seto no Hanayome,TV,Manga,7.89,91206,727.0,366,"Comedy, Parody, Romance, School, Shounen"
karthiga,1974,8,2255153,Female,Glass no Kamen (2005),TV,Manga,8.18,7348,333.0,2523,"Drama, Shoujo"
karthiga,1914,7,2255153,Female,Saiunkoku Monogatari 2nd Season,TV,Light novel,8.16,11924,362.0,2233,"Adventure, Comedy, Drama, Fantasy, Historical, Romance"
karthiga,1735,9,2255153,Female,Naruto: Shippuuden,TV,Manga,8.18,385179,337.0,20,"Action, Adventure, Comedy, Super Power, Martial Arts, Shounen"
karthiga,1698,7,2255153,Female,Nodame Cantabile,TV,Manga,8.41,87363,163.0,381,"Music, Slice of Life, Comedy, Drama, Romance, Josei"
karthiga,1579,9,2255153,Female,Kiniro no Corda: Primo Passo,TV,Visual novel,7.63,36820,1273.0,1138,"Comedy, Drama, Harem, Magic, Music, Romance, School, Shoujo"
karthiga,1571,8,2255153,Female,Ghost Hunt,TV,Novel,7.92,69224,656.0,527,"Comedy, Horror, Mystery, Shoujo, Supernatural"
karthiga,1557,8,2255153,Female,Shounen Onmyouji,TV,Light novel,7.68,14619,1158.0,1793,"Action, Magic, Fantasy, Supernatural, Demons, Historical, Shoujo"
karthiga,966,10,2255153,Female,Crayon Shin-chan,TV,Manga,7.73,17683,1032.0,1937,"Slice of Life, Comedy, Ecchi, School, Seinen"
karthiga,957,8,2255153,Female,Saiunkoku Monogatari,TV,Light novel,8.04,23467,512.0,1313,"Adventure, Fantasy, Comedy, Romance, Historical, Drama"
karthiga,857,10,2255153,Female,Air Gear,TV,Manga,7.64,126326,1233.0,297,"Action, Comedy, Ecchi, Shounen, Sports"
karthiga,853,7,2255153,Female,Ouran Koukou Host Club,TV,Manga,8.34,335137,205.0,69,"Comedy, Harem, Romance, School, Shoujo"
karthiga,721,7,2255153,Female,Princess Tutu,TV,Original,8.21,36501,307.0,916,"Comedy, Drama, Magic, Romance, Fantasy"
karthiga,269,10,2255153,Female,Bleach,TV,Manga,7.9,433097,693.0,18,"Action, Adventure, Comedy, Super Power, Supernatural, Shounen"
karthiga,249,8,2255153,Female,InuYasha,TV,Manga,7.9,181978,697.0,141,"Action, Adventure, Comedy, Historical, Demons, Supernatural, Magic, Romance, Fantasy, Shounen"
karthiga,233,6,2255153,Female,Daa! Daa! Daa!,TV,Manga,7.78,6598,919.0,3045,"Comedy, Sci-Fi, Shoujo"
karthiga,232,6,2255153,Female,Cardcaptor Sakura,TV,Manga,8.21,121898,297.0,292,"Adventure, Comedy, Drama, Magic, Romance, Fantasy, School, Shoujo"
karthiga,210,7,2255153,Female,Ranma ½,TV,Manga,7.85,59911,802.0,623,"Slice of Life, Comedy, Martial Arts, Fantasy"
karthiga,178,7,2255153,Female,Ultra Maniac,TV,Manga,7.26,9663,2594.0,2490,"Magic, Comedy, Romance, School, Shoujo"
karthiga,120,7,2255153,Female,Fruits Basket,TV,Manga,7.77,167968,939.0,222,"Slice of Life, Comedy, Drama, Romance, Fantasy, Shoujo"
karthiga,74,7,2255153,Female,Gakuen Alice,TV,Manga,7.77,33244,941.0,1291,"Comedy, School, Shoujo, Super Power"
karthiga,59,7,2255153,Female,Chobits,TV,Manga,7.53,175388,1546.0,188,"Sci-Fi, Comedy, Drama, Romance, Ecchi, Seinen"
karthiga,21,9,2255153,Female,One Piece,TV,Manga,8.54,423868,91.0,35,"Action, Adventure, Comedy, Super Power, Drama, Fantasy, Shounen"

Total de registros encontrados: 53
```

---

#### Búsqueda por Nombre de usuario y Nombre del anime

Esta opción permite una búsqueda más precisa, especificando tanto el **Nombre de usuario** como el **Nombre del anime** para obtener la calificación específica.

Output obtenido:

```bash
Enviando: karthiga,Kimi ni Todoke 2nd Season,-
Comando recibido: karthiga,Kimi ni Todoke 2nd Season,-
Buscando registros para 'karthiga' que coincida con 'Kimi ni Todoke 2nd Season','-'...

karthiga,9656,8,2255153,Female,Kimi ni Todoke 2nd Season,TV,Manga,8.12,133548,412.0,322,"Slice of Life, Drama, Romance, School, Shoujo"

Total de registros encontrados: 1

```

---

#### Búsqueda por Nombre de usuario y Tipo de anime

Esta opción permite filtrar las calificaciones de un usuario dependiendo del **Tipo de anime**.

```bash
Recibido: Comando recibido: karthiga,-,Novel
Recibido: Buscando registros para 'karthiga' que coincida con '-','Novel'...

karthiga,1571,8,2255153,Female,Ghost Hunt,TV,Novel,7.92,69224,656.0,527,"Comedy, Horror, Mystery, Shoujo, Supernatural"
Recibido: karthiga,1571,8,2255153,Female,Ghost Hunt,TV,Novel,7.92,69224,656.0,527,"Comedy, Horror, Mystery, Shoujo, Supernatural"

Total de registros encontrados: 1
```

---

#### Búsqueda con Nombre de usuario No Existente en el Dataset

Este ejemplo ilustra el comportamiento del sistema cuando se busca un **Nombre de usuario** que no se encuentra en el dataset.

```bash
Enviando: Federico,-,-
Comando recibido: Federico,-,-
No se encontraron registros para la clave 'Federicoaa'.
Recibido: Comando recibido: Federico,-,-
Recibido: No se encontraron registros para la clave 'Federicoaa'.
```
