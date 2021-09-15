import {Component, OnInit} from '@angular/core';
import {TempService} from '../temp.service';
import {SwalService} from '../swal.service';

@Component({
  selector: 'app-information',
  templateUrl: './information.component.html',
  styleUrls: ['./information.component.scss']
})
export class InformationComponent implements OnInit {

  constructor(private tempService: TempService, private swal: SwalService) {
  }

  ngOnInit(): void {
  }

  reEv() {
    this.tempService.resetEverything().then(() => this.swal.ok());
  }

  reInterval() {
    this.tempService.resetIntervals().then(() => this.swal.ok());
  }

  restart() {
    this.tempService.restart().then(() => this.swal.ok());
  }
}
