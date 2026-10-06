#pragma once

template<typename tipo_vertice>
class Vertice
{
private:
  tipo_vertice m_rotulo;

public:
  // explicit previne conversões implicitas do compilador
  explicit Vertice(const tipo_vertice& rotulo)
    : m_rotulo(rotulo) {};

  // retorna uma referencia ao objeto original com permissão apenas de leitura
  const tipo_vertice& get_rotulo() const { return m_rotulo; };
};
