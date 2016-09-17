extern crate gtk;

use gtk::prelude::*;

use gtk::{Button, Window, Builder, Image};

mod image_mgr;

struct Gui {
    window: Window,
    btn_prev: Button,
    btn_next: Button,
    current_img: Image
}

impl Gui {
    fn new(file_path: &str) -> Gui {
        let builder = Builder::new_from_file(file_path);
        let ret = Gui {
            window: builder.get_object("main_frame").expect("Couldn't load main frame"),
            btn_prev: builder.get_object("btn_prev").expect("Couldn't load btn_prev"),
            btn_next: builder.get_object("btn_next").expect("Couldn't load btn_next"),
            current_img: builder.get_object("current_img").expect("Couldn't load current_img")
        };

        ret.window.connect_delete_event(|_, _| {
            gtk::main_quit();
            Inhibit(false)
        });

        ret.btn_prev.connect_clicked(move |_| {
            println!("Clicked prev");
        });

        ret.btn_next.connect_clicked(move |_| {
            println!("Clicked next");
        });

        return ret;
    }

    fn show(&mut self) {
        self.window.show_all();
    }
}

fn main() {
    if gtk::init().is_err() {
        println!("Failed to initialize GTK.");
        return;
    }

    let mut gui = Gui::new("/home/john/git/face-recognition/src/ui.glade");
    gui.show();

    let mut face = image_mgr::FaceImage::new_from_pgm("/home/john/git/face-recognition/data/orl_faces/s1/1.pgm");

    unsafe {
        gui.current_img.set_from_pixbuf(face.to_gdk_pixbuf().as_ref());
    }
    gtk::main();
}
