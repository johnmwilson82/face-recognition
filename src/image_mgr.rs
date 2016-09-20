extern crate rgsl;
extern crate image;
extern crate gdk_pixbuf;
extern crate gdk_pixbuf_sys;

use self::image::GrayImage;

use std::fs::{self, File, DirEntry, ReadDir};
use std::io::{BufReader, Read, Result};
use std::collections::HashMap;
use std::str;
use std::str::FromStr;
use std::iter;
use std::path::Path;
use std::ffi::OsStr;

use self::gdk_pixbuf::Pixbuf;


struct FaceImage {
    data_mat: rgsl::MatrixF32,
    width: u32,
    height: u32,
    class_id: u32
}

impl FaceImage {

    pub fn get_width(&self) -> u32 {
        self.width
    }

    pub fn get_height(&self) -> u32 {
        self.height
    }

    pub fn to_image(&self) -> GrayImage {
        let mut imgbuf = GrayImage::new(self.width, self.height);
        for (x, y, pixel) in imgbuf.enumerate_pixels_mut() {
            *pixel = image::Luma(
                [(self.data_mat.get(x as usize, y as usize) * 255.0) as u8]);
        }
        imgbuf
    }

    pub fn to_gdk_pixbuf(&self) -> Option<Pixbuf> {
        // Convert the greyscale image buffer to RGB
        let v: Vec<u8> = self.to_image()
                             .into_raw()
                             .into_iter()
                             .flat_map(|x| iter::repeat(x).take(3))
                             .collect::<Vec<u8>>();

        let pixbuf = Pixbuf::new_from_vec(
            v,
            self::gdk_pixbuf_sys::GDK_COLORSPACE_RGB,
            false, 8, self.width as i32, self.height as i32, self.width as i32 * 3);
        Some(pixbuf)
    }

    fn matrix_from_pixel_data(
        width: &u32, height: &u32, maxval: &u32, data: &[u8]) ->
            rgsl::MatrixF32
    {
        let pixel_mat = rgsl::MatrixF32::new(
                    *width as usize, *height as usize).unwrap();
        let (mut x, mut y) = (0, 0);

        for pixel in data.iter() {
            let fpixel = *pixel as f32 / *maxval as f32;
            pixel_mat.set(x, y, fpixel);
            x += 1;
            if x == *width as usize {
                x = 0;
                y += 1;
            }
        }

        pixel_mat
    }

    pub fn new_from_pgm(file_path: &Path, class_id: u32) -> FaceImage {
        let mut file = BufReader::new(File::open(file_path).unwrap());

        let mut file_data = Vec::<u8>::new();
        let _ = file.read_to_end(&mut file_data);

        let mut i = 0;
        let mut buf = String::new();
        let mut header_data = Vec::<String>::new();
        let mut consumed_bytes = 0;

        for b in &file_data {
            match *b as char {
                ' ' | '\t' | '\n' => {
                    match i {
                        0...3 => {
                            consumed_bytes += buf.len() + 1;
                            if buf.len() > 0 {
                                i += 1;
                            }
                            header_data.push(buf.clone());
                            buf.clear();
                        },
                        _ => break
                    }
                },

                _ => buf.push(*b as char)
            };
        };

        assert!(header_data[0] == "P5");
        let width = u32::from_str(&header_data[1]).unwrap();
        let height = u32::from_str(&header_data[2]).unwrap();
        let maxval = u32::from_str(&header_data[3]).unwrap();

        let (_, pixel_data) = file_data.split_at(consumed_bytes);

        let mut pixel_mat = FaceImage::matrix_from_pixel_data(
                                &width, &height, &maxval, pixel_data);

        println!("Opened pgm {}, size {}, {}, maxval {}",
                file_path.display(),
                width, height, maxval);

        return FaceImage {
            data_mat: pixel_mat,
            width: width,
            height: height,
            class_id: class_id
        };
    }
}

pub struct FaceCatalogue {
    face_images: Vec<FaceImage>,
    names_map: HashMap<u32, String>,
    selected: usize
}

impl FaceCatalogue {
    pub fn new(dir: &Path) -> Result<FaceCatalogue> {
        let mut face_images = Vec::<FaceImage>::new();
        let mut names_map = HashMap::new();
        let mut class_id: u32 = 0;
        for model_dir in try!(fs::read_dir(dir)) {
            let dir_path = try!(model_dir).path();
            if !dir_path.is_dir() {
                continue;
            }
            let dir_path2 = dir_path.clone();
            let dir_name = dir_path2.file_name().and_then(OsStr::to_str);
            names_map.insert(class_id, String::from(dir_name.unwrap_or("unknown")));
            for file in try!(fs::read_dir(dir_path)) {
                face_images.push(FaceImage::new_from_pgm(&file.unwrap().path(), class_id));
            }
            class_id += 1;
        }

        Ok(FaceCatalogue {
            face_images: face_images,
            names_map: names_map,
            selected: 0
        })
    }

    pub fn next(&mut self) {
        self.selected += 1;
        self.selected %= self.face_images.len();
    }

    pub fn prev(&mut self) {
        if self.selected == 0 {
            self.selected = self.face_images.len() - 1;
        } else
        {
            self.selected -= 1;
        }
    }

    pub fn get_sel_gdk_pixbuf(&self) -> Option<Pixbuf> {
        self.face_images[self.selected].to_gdk_pixbuf()
    }
}
