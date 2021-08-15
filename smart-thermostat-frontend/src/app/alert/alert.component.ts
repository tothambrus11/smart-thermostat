import {Component, OnInit} from '@angular/core';
import {AlertService} from '../alert.service';

@Component({
  selector: 'app-alert',
  templateUrl: './alert.component.html',
  styleUrls: ['./alert.component.scss']
})
export class AlertComponent implements OnInit {

  constructor(public alertService: AlertService) {
  }

  ngOnInit(): void {
  }

  cancel() {
    if (this.alertService.alertMessage!.onCancel) {
      this.alertService.alertMessage!.onCancel();
    }
    this.alertService.alertMessage = undefined;
  }

  ok() {
    if (this.alertService.alertMessage!.onOk) {
      this.alertService.alertMessage!.onOk();
    }
    this.alertService.alertMessage = undefined;
  }
}
