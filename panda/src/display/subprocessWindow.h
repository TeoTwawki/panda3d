// Filename: subprocessWindow.h
// Created by:  drose (11Jul09)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) Carnegie Mellon University.  All rights reserved.
//
// All use of this software is subject to the terms of the revised BSD
// license.  You should have received a copy of this license along
// with this source code in a file named "LICENSE."
//
////////////////////////////////////////////////////////////////////

#ifndef SUBPROCESSWINDOW_H
#define SUBPROCESSWINDOW_H

#include "pandabase.h"

// For now, a simple trigger whether to enable the subprocess window
// support.  We only build it on OSX, and only when the plugin is
// enabled; because this is (presently) the only case where it's
// useful.
#if defined(HAVE_P3D_PLUGIN) && defined(IS_OSX)
#define SUPPORT_SUBPROCESS_WINDOW 1
#else
#undef SUPPORT_SUBPROCESS_WINDOW
#endif

#ifdef SUPPORT_SUBPROCESS_WINDOW

#include "graphicsWindow.h"
#include "graphicsBuffer.h"
#include "texture.h"
#include "subprocessWindowBuffer.h"

////////////////////////////////////////////////////////////////////
//       Class : SubprocessWindow
// Description : This is a special "window" that actually renders to
//               an offscreen buffer, copies the pixels to RAM, and
//               then ships them to a parent process via shared memory
//               for rendering to the window.
//
//               This whole nonsense is necessary because OSX doesn't
//               allow child processes to draw to, or attach windows
//               to, windows created in the parent process.  There's a
//               rumor that 10.6 fixes this nonsense; this will remain
//               to be seen.
////////////////////////////////////////////////////////////////////
class SubprocessWindow : public GraphicsWindow {
public:
  SubprocessWindow(GraphicsEngine *engine, GraphicsPipe *pipe, 
                   const string &name,
                   const FrameBufferProperties &fb_prop,
                   const WindowProperties &win_prop,
                   int flags,
                   GraphicsStateGuardian *gsg,
                   GraphicsOutput *host,
                   const string &filename);
  virtual ~SubprocessWindow();

  virtual void process_events();

  virtual bool begin_frame(FrameMode mode, Thread *current_thread);
  virtual void end_frame(FrameMode mode, Thread *current_thread);
  virtual void begin_flip();

  virtual void set_properties_now(WindowProperties &properties);

protected:
  virtual void close_window();
  virtual bool open_window();

private:
  PT(GraphicsBuffer) _buffer;
  PT(Texture) _texture;

  int _fd;
  size_t _mmap_size;
  string _filename;
  SubprocessWindowBuffer *_swbuffer;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    GraphicsWindow::init_type();
    register_type(_type_handle, "SubprocessWindow",
                  GraphicsWindow::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#include "subprocessWindow.I"

#endif  // SUPPORT_SUBPROCESS_WINDOW

#endif
