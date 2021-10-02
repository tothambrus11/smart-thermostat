import {Injectable} from '@angular/core';

export interface AlertMessage {
  title: string;
  message: string;
  showCancelButton?: boolean;
  showOkButton?: boolean;
  showCloseButton?: boolean;
  cancelButtonText?: string;
  closeButtonText?: string;
  okButtonText?: string;
  icon: string;
  onCancel?: () => void;
  onOk?: () => void;
  onClose?: () => void;
  isWarning?: boolean;
}

@Injectable({
  providedIn: 'root'
})
export class AlertService {
  alertMessage?: AlertMessage;

  constructor() {

  }

  alert(alertMessage: AlertMessage) {
    alertMessage.cancelButtonText ||= 'Mégse';
    alertMessage.okButtonText ||= 'Ok';
    alertMessage.closeButtonText ||= 'Bezárás'
    this.alertMessage = alertMessage;
  }

}
