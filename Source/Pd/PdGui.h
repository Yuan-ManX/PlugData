/*
 // Copyright (c) 2015-2018 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */
#pragma once

#include <JuceHeader.h>

#include "PdArray.h"
#include "PdAtom.h"
#include "PdObject.h"

namespace pd
{
class Label;

// ==================================================================================== //
//                                          GUI                                         //
// ==================================================================================== //

//! @brief The Pd GUI objects.
//! @details The Instance is a wrapper for the Pd native GUI. The lifetime of the internal\n
//! GUI is not guaranteed by the class.
//! @see Instance, Patch, Object
class Gui : public Object
{
 public:
    
  //! @brief The type of the GUI.
  inline Type getType() const noexcept override { return type; }

  static Type getType(void* ptr) noexcept;

  //! @brief If the GUI is an IEM's GUI.
  bool isIEM() const noexcept { return (type != Type::Undefined) && (type < Type::Comment); }

  //! @brief If the GUI is an Atom GUI (AtomNumber or AtomSymbol).
  bool isAtom() const noexcept { return (type == Type::AtomNumber) || (type == Type::AtomSymbol); }

  //! @brief Get the font height.
  float getFontHeight() const noexcept;

  //! @brief Get the font name.
  std::string getFontName() const;

  float getMinimum() const noexcept;
  float getMaximum() const noexcept;

  void setMinimum(float value) noexcept;
  void setMaximum(float value) noexcept;

  void setSendSymbol(const std::string& symbol) const noexcept;
  void setReceiveSymbol(const std::string& symbol) const noexcept;

  std::string getSendSymbol() noexcept;
  std::string getReceiveSymbol() noexcept;

  float getValue() const noexcept;
  float getPeak() const noexcept;

  void setValue(float value) noexcept;

  size_t getNumberOfSteps() const noexcept;

  unsigned int getBackgroundColor() const noexcept;

  unsigned int getForegroundColor() const noexcept;

  std::string getSymbol() const noexcept;

  void setSymbol(std::string const& value) noexcept;

  void click() noexcept;

  std::array<int, 4> getBounds() const noexcept override;
  void setSize(int w, int h) noexcept;

  bool jumpOnClick() const noexcept;

  bool isLogScale() const noexcept;
  void setLogScale(bool log) noexcept;

  Array getArray() const noexcept;

  Label getLabel() const noexcept;
  Point<int> getLabelPosition(Rectangle<int> bounds) const noexcept;

  Patch getPatch() const noexcept;

  std::vector<Atom> getList() const noexcept;

  void setList(std::vector<Atom> const& value) noexcept;

  Gui(void* ptr, Patch* patch, Instance* instance) noexcept;

 private:
  Type type = Type::Undefined;
  friend class Patch;
};

// ==================================================================================== //
//                                      LABEL                                           //
// ==================================================================================== //

class Label
{
 public:
  Label() noexcept;
  Label(Label const& other) noexcept;
  Label(std::string text, unsigned int color, int x, int y, std::string fontname, float fontheight) noexcept;

  std::string getText() const noexcept { return m_text; }
  unsigned int getColor() const noexcept { return m_color; }
  std::array<int, 2> getPosition() const noexcept { return m_position; }

  //! @brief Get the font height.
  float getFontHeight() const noexcept;

  //! @brief Get the font name.
  std::string getFontName() const;

 private:
  void* ptr;
  std::string const m_text;
  unsigned int const m_color;
  std::array<int, 2> const m_position;
  std::string m_font_name;
  float m_font_height;
  friend class Gui;
};
}  // namespace pd