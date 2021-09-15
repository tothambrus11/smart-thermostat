import { Injectable } from '@angular/core';
import Swal from 'sweetalert2';

@Injectable({
  providedIn: 'root'
})
export class SwalService {

  constructor() { }

  ok() {
    Swal.fire({
      titleText: "Ok",
      position: "bottom-right",
      toast: true,
      icon: 'success',
      showConfirmButton: false,
      timer: 3000,
    })
  }
}
