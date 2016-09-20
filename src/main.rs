extern crate gtk;
extern crate gdk_pixbuf;

use gtk::prelude::*;
use std::path::Path;
use gdk_pixbuf::Pixbuf;
use std::cell::RefCell;
use std::rc::Rc;
use gtk::{Button, Window, Builder, Image};

mod image_mgr;

struct Gui {
    window: Window,
    btn_prev: Button,
    btn_next: Button,
    current_img: Image,
    face_catalogue: RefCell<image_mgr::FaceCatalogue>
}

impl Gui {
    fn new(file_path: &str, face_catalogue: RefCell<image_mgr::FaceCatalogue>) -> Rc<Gui> {
        let builder = Builder::new_from_file(file_path);
        let ret = Rc::new(Gui {
            window: builder.get_object("main_frame").expect("Couldn't load main frame"),
            btn_prev: builder.get_object("btn_prev").expect("Couldn't load btn_prev"),
            btn_next: builder.get_object("btn_next").expect("Couldn't load btn_next"),
            current_img: builder.get_object("current_img").expect("Couldn't load current_img"),
            face_catalogue: face_catalogue
        });

        ret.window.connect_delete_event(|_, _| {
            gtk::main_quit();
            Inhibit(false)
        });

        {
            let inst = ret.clone();
            ret.btn_prev.connect_clicked(move |_| {
                inst.face_catalogue.borrow_mut().prev();
                inst.current_img.set_from_pixbuf(inst.face_catalogue.borrow().get_sel_gdk_pixbuf().as_ref());
            });
        }

        {
            let inst = ret.clone();
            ret.btn_next.connect_clicked(move |_| {
                inst.face_catalogue.borrow_mut().next();
                inst.current_img.set_from_pixbuf(inst.face_catalogue.borrow().get_sel_gdk_pixbuf().as_ref());
            });
        }

        ret.current_img.set_from_pixbuf(ret.face_catalogue.borrow().get_sel_gdk_pixbuf().as_ref());
        ret.window.show_all();

        return ret;
    }

}

fn main() {
    if gtk::init().is_err() {
        println!("Failed to initialize GTK.");
        return;
    }

    let mut face_catalogue = image_mgr::FaceCatalogue::new(Path::new("/home/john/git/face-recognition/data/orl_faces"));

    let mut gui = Gui::new("/home/john/git/face-recognition/src/ui.glade", RefCell::new(face_catalogue));

    gtk::main();
}
