#include <iostream>
#include <SFML/Graphics.hpp>
//#include <SFML/System.hpp>
//#include <SFML/Window.hpp>
using namespace sf;
using namespace std;
class Tablero {
public:
    int size = 8;

    void Draw(RenderWindow& ventana) {
        RectangleShape casilla;
        RectangleShape tablero({ 600,600 });
        Texture tabl;
        tablero.setPosition(100, 0);
        tabl.loadFromFile("texturas/tablero.png");
        tablero.setTexture(&tabl);
        ventana.draw(tablero);

    }

    void Select(RenderWindow& ventana, int x, int y) {
        RectangleShape casilla;
        casilla.setSize(Vector2f(67.5f, 67.5f)); // Ajustar el tamaño para un mapa de 540x540
        //casilla.setFillColor(Color::Magenta);
        Texture cur;
        cur.loadFromFile("texturas/cursor.png");
        casilla.setTexture(&cur);
        casilla.setPosition(Vector2f(67.5f * x + 130, 67.5f * y + 10)); // Modificar +130 en la coordenada X
        ventana.draw(casilla);
    }

};

class Dama {
public:
    float r = 30;
    int x;
    int y;
    bool isAlive;
    bool isCrowned = false;
    char team = ' ';
    Texture texture; // Agregar una textura
    Sprite sprite;
    Color color;


    void Draw(RenderWindow& ventana) {
        if (isAlive) {
            CircleShape shape(r);
            Texture fich;
            if (team == 'B') {
                fich.loadFromFile("texturas/blanca.png");
            }
            else {
                fich.loadFromFile("texturas/negra.png");
            }
            //shape.setFillColor(color);
            if (isCrowned and team == 'N') {
                fich.loadFromFile("texturas/Rnegra.png");
            }
            else if (isCrowned and team == 'B') {
                fich.loadFromFile("texturas/Rblanca.png");
            }
            shape.setTexture(&fich);
            shape.setPosition(Vector2f((x * 67.5f + (67.5f - r * 2) / 2) + 130, (y * 67.5f + (67.5f - 2 * r) / 2) + 30));

            ventana.draw(shape);
        }
    }
};

void Init(RenderWindow& ventana, Dama* rojas, Dama* negras) {
    int m = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = (int)!(i % 2 & 1); j < 8; j += 2) {
            negras[m].isAlive = true;
            negras[m].x = j;
            negras[m].y = i;
            m++;
        }

    }
    m = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = (int)(i % 2 & 1); j < 8; j += 2) {
            rojas[m].isAlive = true;
            rojas[m].x = j;
            rojas[m].y = 7 - i;
            m++;
        }

    }
}

void muertas(RenderWindow& ventana, int rdead, int ndead) {
    RectangleShape moridosB({ 40,40 }); // Establece el radio directamente en el constructor
    Texture BM;
    if (BM.loadFromFile("texturas/muertab.png")) { // Verifica si la carga de la textura fue exitosa
        moridosB.setTexture(&BM);
        int y = 500;
        moridosB.setPosition(20, y);

        for (int i = 0; i < rdead; i++) {
            ventana.draw(moridosB);
            y -= 40; // Puedes usar el operador de incremento para simplificar esto
            moridosB.setPosition(20, y); // Establece la posición en cada iteración
        }
    }
    RectangleShape moridosN({ 40,40 }); // Establece el radio directamente en el constructor
    Texture NM;
    if (NM.loadFromFile("texturas/muertan.png")) { // Verifica si la carga de la textura fue exitosa
        moridosN.setTexture(&NM);
        int x = 500;
        moridosN.setPosition(700, x);
        for (int i = 0; i < ndead; i++) {
            ventana.draw(moridosN);
            x -= 40; // Puedes usar el operador de incremento para simplificar esto
            moridosN.setPosition(700, x); // Establece la posición en cada iteración
        }
    }
}


Dama* Find(int x, int y, Dama* rojas, Dama* negras) {
    for (int i = 0; i < 12; i++) {
        if (rojas[i].x == x && rojas[i].y == y) {
            if (rojas[i].isAlive) {
                return &rojas[i];
            }
        }
        if (negras[i].x == x && negras[i].y == y) {
            if (negras[i].isAlive) {
                return &negras[i];
            }
        }
    }
    return NULL;
}

void Kill(int x, int y, Dama* rojas, Dama* negras, int* turno) {
    Find(x, y, rojas, negras)->isAlive = false;
    return;
}

int Move(int x, int y, Dama* selected, Dama* rojas, Dama* negras, int* turno) {
    if (x >= 0 && x < 8 && y >= 0 && y < 8)
    {
        if (selected->team == 'B' || selected->team == 'N' && selected->isCrowned) {
            if (x == selected->x - 1 && y == selected->y - 1) {
                if (!Find(x, y, rojas, negras)) {
                    *turno = ((*turno == 1) ? 2 : 1);
                    selected->x = x;
                    selected->y = y;
                    return 1;
                }
            }
            if (x == selected->x + 1 && y == selected->y - 1) {
                if (!Find(x, y, rojas, negras)) {
                    *turno = ((*turno == 1) ? 2 : 1);
                    selected->x = x;
                    selected->y = y;
                    return 1;
                }
            }
            if (x == selected->x - 2 && y == selected->y - 2) {
                if (!Find(x, y, rojas, rojas) && Find(x + 1, y + 1, rojas, negras) != NULL && Find(x + 1, y + 1, rojas, negras)->team != selected->team) {
                    *turno = ((*turno == 1) ? 2 : 1);
                    Kill(x + 1, y + 1, rojas, negras, turno);
                    selected->x = x;
                    selected->y = y;
                    return 1;
                }
            }
            if (x == selected->x + 2 && y == selected->y - 2) {
                if (!Find(x, y, rojas, negras) && Find(x - 1, y + 1, negras, negras) != NULL && Find(x - 1, y + 1, rojas, negras)->team != selected->team) {
                    *turno = ((*turno == 1) ? 2 : 1);
                    Kill(x - 1, y + 1, rojas, negras, turno);
                    selected->x = x;
                    selected->y = y;
                    return 1;
                }
            }
        }
        if (selected->team == 'N' || selected->team == 'B' && selected->isCrowned) {
            if (x == selected->x - 1 && y == selected->y + 1) {
                if (!Find(x, y, rojas, negras)) {
                    *turno = ((*turno == 1) ? 2 : 1);
                    selected->x = x;
                    selected->y = y;
                    return 1;
                }
            }
            if (x == selected->x + 1 && y == selected->y + 1) {
                if (!Find(x, y, rojas, negras)) {
                    *turno = ((*turno == 1) ? 2 : 1);
                    selected->x = x;
                    selected->y = y;
                    return 1;
                }
            }
            if (x == selected->x - 2 && y == selected->y + 2) {
                if (!Find(x, y, rojas, negras) && Find(x + 1, y - 1, rojas, negras) != NULL && Find(x + 1, y - 1, rojas, negras)->team != selected->team) {
                    *turno = ((*turno == 1) ? 2 : 1);
                    selected->x = x;
                    selected->y = y;
                    Kill(x + 1, y - 1, rojas, negras, turno);
                    return 1;
                }
            }
            if (x == selected->x + 2 && y == selected->y + 2) {
                if (!Find(x, y, rojas, negras) && Find(x - 1, y - 1, rojas, negras) != NULL && Find(x - 1, y - 1, rojas, negras)->team != selected->team) {
                    *turno = ((*turno == 1) ? 2 : 1);
                    selected->x = x;
                    selected->y = y;
                    Kill(x - 1, y - 1, rojas, negras, turno);
                    return 1;
                }
            }
        }
    }

    return 0;
}
class Node
{
public:
    Node* enlaces[96];
    Tablero _tablero;
    Dama _Rojas[12];
    Dama _Negras[12];
    bool inicializado = 0;

    Node(Tablero tablero, Dama Rojas[12], Dama Negras[12]) //copiamos el tablero actual y las posiciones de las damas
    {
        _tablero = tablero;
        inicializado = 1;

        for (int x = 0; x < 12; x++)
        {
            _Rojas[x] = Rojas[x];
            _Negras[x] = Negras[x];
        }
        for (int x = 0; x < 48; x++)
        {
            enlaces[x] = 0;
        }
    }
};

struct Arbol
{
    Node* _root;
    Dama Rojas_finales[12];
    Dama Negras_finales[12];
    int _nivel;
    int movimientos_x_n[4] = { -1,1,-2,2 };
    int movimientos_y_n[4] = { 1,1,2,2 };

    int movimientos_x_coronada[8] = { -1,1,-2,2,-1,1,-2,2 };
    int movimientos_y_coronada[8] = { 1,1,2,2,-1,-1,-2,-2 };

    int movimientos_x_r[4] = { -1,1,-2,2 };
    int movimientos_y_r[4] = { -1,-1,-2,-2 };



    int cambios = 0;

    Arbol(Tablero tablero, Dama Rojas[12], Dama Negras[12], int nivel)
    {
        _root = new Node(tablero, Rojas, Negras);//inicializamos la raiz con el tablero y las damas actuales
        _nivel = nivel;
    }

    ~Arbol() {
        // Llama a una función de limpieza recursiva para liberar la memoria de los nodos del árbol
        LimpiarArbol(_root);
    }

    void LimpiarArbol(Node* nodo) {
        if (nodo) {
            for (int i = 0; i < 48; i++) {
                LimpiarArbol(nodo->enlaces[i]);
            }
            delete nodo;
        }
    }
    int funcion(Node* Actual)
    {
        int rojas_vivas = 0;
        int negras_vivas = 0;

        for (int x = 0; x < 12; x++)
        {
            if (Actual->_Rojas[x].isAlive)
            {
                rojas_vivas++;
                if (Actual->_Rojas[x].isCrowned) {
                    // Aumentar el valor para las fichas rojas coronadas
                    rojas_vivas += 10;
                }
            }
            else {
                rojas_vivas--;
            }
            if (Actual->_Negras[x].isAlive)
            {
                negras_vivas++;
                if (Actual->_Negras[x].isCrowned) {
                    // Aumentar el valor para las fichas negras coronadas
                    negras_vivas += 10;
                }
            }
            else {
                negras_vivas--;
            }

        }

        return negras_vivas - rojas_vivas;
    }

    void simular_movimiento(int nivel, Node* Actual)
    {
        Dama aux_rojas[12];
        Dama aux_negro[12];
        for (int x = 0; x < 12; x++)
        {
            aux_rojas[x] = Actual->_Rojas[x];
            aux_negro[x] = Actual->_Negras[x];
        }
        Dama* SelectedPiece;
        int turno = 0;
        int turno_ini = 0;
        int posx = 0;
        int posy = 0;
        int i = 0;
        if (nivel == _nivel)
        {
            return;
        }
        for (int x = 0; x < 12; x++)
        {
            if (nivel % 2 == 1) //si es el turno de las rojas asignamos la posicion a la ficha roja x
            {
                turno = 1;
                turno_ini = 1;
                posx = Actual->_Rojas[x].x;
                posy = Actual->_Rojas[x].y;
                SelectedPiece = Find(posx, posy, Actual->_Rojas, Actual->_Negras);
            }
            else //caso contrario lo hacemos con las negras
            {
                turno = 2;
                turno_ini = 2;
                posx = Actual->_Negras[x].x;
                posy = Actual->_Negras[x].y;
                SelectedPiece = Find(posx, posy, Actual->_Rojas, Actual->_Negras);
            }
            if (SelectedPiece != NULL) //si hemos encontrado una ficha en esa posicion (si no esta muerta)
            {
                if (SelectedPiece->isCrowned)
                {
                    for (int y = 0; y < 8; y++)
                    {
                        if (turno_ini == 2) //vemos si existe un movimento posible como ficha negra y si existe lo hacemos y luego reseteamos los valores
                        {
                            if (Move(posx + movimientos_x_coronada[y], posy + movimientos_y_coronada[y], SelectedPiece, Actual->_Rojas, Actual->_Negras, &turno) == 1)
                            {
                                cambios++;
                                nivel++;
                                Actual->enlaces[i] = new Node(Actual->_tablero, Actual->_Rojas, Actual->_Negras);
                                simular_movimiento(nivel, Actual->enlaces[i]);
                                i++;
                                nivel--;
                                for (int x = 0; x < 12; x++)
                                {
                                    Actual->_Rojas[x] = aux_rojas[x];
                                    Actual->_Negras[x] = aux_negro[x];
                                }
                            }
                        }
                        else if (turno_ini == 1) //lo mismo para las rojas
                        {
                            if (Move(posx + movimientos_x_coronada[y], posy + movimientos_y_coronada[y], SelectedPiece, Actual->_Rojas, Actual->_Negras, &turno) == 1)
                            {
                                cambios++;
                                nivel++;
                                Actual->enlaces[i] = new Node(Actual->_tablero, Actual->_Rojas, Actual->_Negras);
                                simular_movimiento(nivel, Actual->enlaces[i]);
                                i++;
                                nivel--;
                                for (int x = 0; x < 12; x++)
                                {
                                    Actual->_Rojas[x] = aux_rojas[x];
                                    Actual->_Negras[x] = aux_negro[x];
                                }
                            }
                        }
                    }
                }
                else
                {
                    for (int y = 0; y < 4; y++)
                    {
                        if (turno_ini == 2) //vemos si existe un movimento posible como ficha negra y si existe lo hacemos y luego reseteamos los valores
                        {
                            if (Move(posx + movimientos_x_n[y], posy + movimientos_y_n[y], SelectedPiece, Actual->_Rojas, Actual->_Negras, &turno) == 1)
                            {
                                cambios++;
                                nivel++;
                                //std::cout << "Nivel antes del cambio: " << nivel << std::endl;
                                //std::cout << "cambio nro " << cambios << " cambio desde: " << posx << " " << posy << " hasta " << posx + movimientos_x_n[y] << " " << posy + movimientos_y_n[y] << " en el turno " << turno_ini << std::endl;
                                Actual->enlaces[i] = new Node(Actual->_tablero, Actual->_Rojas, Actual->_Negras);
                                simular_movimiento(nivel, Actual->enlaces[i]);
                                i++;
                                nivel--;
                                for (int x = 0; x < 12; x++)
                                {
                                    Actual->_Rojas[x] = aux_rojas[x];
                                    Actual->_Negras[x] = aux_negro[x];
                                }
                            }
                        }
                        else if (turno_ini == 1) //lo mismo para las rojas
                        {
                            if (Move(posx + movimientos_x_r[y], posy + movimientos_y_r[y], SelectedPiece, Actual->_Rojas, Actual->_Negras, &turno) == 1)
                            {
                                cambios++;
                                nivel++;
                                //std::cout << "Nivel antes del cambio: " << nivel << std::endl;
                                //std::cout << "cambio nro " << cambios << " cambio desde: " << posx << " " << posy << " hasta " << posx + movimientos_x_r[y] << " " << posy + movimientos_y_r[y] << " en el turno " << turno_ini << std::endl;
                                Actual->enlaces[i] = new Node(Actual->_tablero, Actual->_Rojas, Actual->_Negras);
                                simular_movimiento(nivel, Actual->enlaces[i]);
                                i++;
                                nivel--;
                                for (int x = 0; x < 12; x++)
                                {
                                    Actual->_Rojas[x] = aux_rojas[x];
                                    Actual->_Negras[x] = aux_negro[x];
                                }
                            }
                        }


                    }
                }

            }


        }
    }
    int min_max(int nivel, Node* Actual, int alfa, int beta)
    {
        //int alfa = -100000;
        //int beta = 1000000;
        //si alfa es mayor o igual que beta 
        if (Actual)
        {
            if (nivel == _nivel - 1)
            {
                if (Actual->inicializado)
                {
                    return funcion(Actual);
                }
                else
                {
                    if (nivel % 2 == 1)
                    {
                        return 0;
                    }
                    else
                    {
                        return 10000;
                    }
                }

            }
            if (nivel == 1)
            {
                int maximo = 0;
                int minimo = 10000;
                int aux;
                for (int x = 0; x < 48; x++)
                {
                    if (alfa >= beta)
                    {
                        break;
                    }
                    nivel++;
                    aux = min_max(nivel, Actual->enlaces[x], alfa, beta);
                    nivel--;
                    if (nivel % 2 == 1)
                    {
                        if (aux < minimo)
                        {
                            for (int x = 0; x < 12; x++)
                            {
                                Rojas_finales[x] = Actual->_Rojas[x];
                                Negras_finales[x] = Actual->_Negras[x];
                            }
                            minimo = aux;
                            beta = minimo;
                        }
                    }
                    else
                    {
                        if (aux > maximo)
                        {
                            for (int x = 0; x < 12; x++)
                            {
                                Rojas_finales[x] = Actual->enlaces[x]->_Rojas[x];
                                Negras_finales[x] = Actual->enlaces[x]->_Negras[x];
                            }
                            maximo = aux;
                            alfa = maximo;
                        }
                    }

                }
                if (nivel % 2 == 1)
                {
                    return minimo;
                }
                else
                {
                    return maximo;
                }
            }
            else
            {
                int maximo = 0;
                int minimo = 10000;
                int aux;

                for (int x = 0; x < 48; x++)
                {
                    if (alfa >= beta)
                    {
                        break;
                    }
                    nivel++;
                    aux = min_max(nivel, Actual->enlaces[x], alfa, beta);
                    nivel--;
                    if (nivel % 2 == 1)
                    {
                        if (aux < minimo)
                        {
                            minimo = aux;
                            beta = minimo;
                        }
                    }
                    else
                    {
                        if (aux > maximo)
                        {
                            maximo = aux;
                            alfa = maximo;
                        }
                    }

                }
                if (nivel % 2 == 1)
                {
                    return minimo;
                }
                else
                {
                    return maximo;
                }
            }

        }

    }

};

int main()
{
    int nivel;
    std::cout << "Ingrese un nivel" << std::endl;
    std::cin >> nivel;
    ContextSettings settings;
    settings.antialiasingLevel = 16.0;
    RenderWindow ventana(VideoMode(800, 600), "Checkers");
    ventana.setFramerateLimit(30);
    Event event;
    Tablero tablero;
    Dama Rojas[12];
    Dama Negras[12];
    bool selected = false;
    Dama* SelectedPiece = NULL;
    int turno = 1;
    bool click = false;


    RectangleShape fondo({ 800,600 });
    fondo.setFillColor(Color(120, 79, 72));

    for (int i = 0; i < 12; i++) {
        Negras[i].color = Color::Black;
        Negras[i].team = 'N';
        Rojas[i].team = 'B';
        Rojas[i].color = Color::Red;
    }


    Init(ventana, Rojas, Negras);

    while (ventana.isOpen()) {


        int ndead = 0, rdead = 0;

        for (int i = 0; i < 12; i++) {
            if (!(Negras[i].isAlive)) {
                ndead++;
            }
        }
        for (int i = 0; i < 12; i++) {
            if (!(Rojas[i].isAlive)) {
                rdead++;
            }
        }
        if (rdead == 12) {
            std::cout << "La IA ha ganado";
            ventana.close();
        }
        if (ndead == 12) {
            std::cout << "El jugador ha ganado";
            ventana.close();
        }

        if (turno == 1)
        {
            while (ventana.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    ventana.close();
                }

                if (event.type == Event::MouseButtonReleased) {
                    if (event.mouseButton.button == Mouse::Left) {
                        selected = !selected;
                    }
                }
            }

            ventana.clear();
            ventana.draw(fondo);
            tablero.Draw(ventana);
            muertas(ventana, rdead, ndead);

            for (int i = 0; i < 12; i++) {
                Negras[i].Draw(ventana);
                Rojas[i].Draw(ventana);
            }
            if (SelectedPiece != NULL) {
                tablero.Select(ventana, SelectedPiece->x, SelectedPiece->y);
            }


            if (selected) {
                int x;
                if (sf::Mouse::getPosition(ventana).x > 100) {
                    x = (sf::Mouse::getPosition(ventana).x - 100) / 75;
                }
                else {
                    x = -1;
                }
                int y = Mouse::getPosition(ventana).y / 75;
                if (x != -1 && Find(x, y, Rojas, Negras) && (Find(x, y, Rojas, Negras)->color == Color::Red && turno == 1 || Find(x, y, Rojas, Negras)->color == Color::Black && turno == 2)) {
                    if (Find(x, y, Rojas, Negras) == SelectedPiece) {
                        SelectedPiece = NULL;
                    }
                    else {
                        SelectedPiece = Find(x, y, Rojas, Negras);
                    }

                    selected = false;
                }
                else if (SelectedPiece != NULL && Move(x, y, SelectedPiece, Rojas, Negras, &turno)) {
                    selected = false;
                    SelectedPiece = NULL;
                }
                selected = false;
            }

            for (int i = 0; i < 12; i++) {
                if (Rojas[i].y == 0) {
                    Rojas[i].isCrowned = true;
                }
                if (Negras[i].y == 7) {
                    Negras[i].isCrowned = true;
                }
            }


            ventana.display();
        }
        else
        {
            Arbol* Minmax = new Arbol(tablero, Rojas, Negras, nivel);
            Minmax->simular_movimiento(0, Minmax->_root);
            Minmax->min_max(0, Minmax->_root, -1000000, 1000000);
            for (int x = 0; x < 12; x++)
            {
                Rojas[x] = Minmax->Rojas_finales[x];
                Negras[x] = Minmax->Negras_finales[x];
            }
            turno = 1;

            // Liberar la memoria utilizando delete
            //Minmax->~Arbol();
            delete Minmax;

        }
    }
    return 0;
}
