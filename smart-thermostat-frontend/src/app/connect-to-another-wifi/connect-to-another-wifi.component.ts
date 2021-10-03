import {Component, Input, OnInit} from '@angular/core';
import {AlertService} from '../alert.service';
import {TempService} from '../temp.service';

@Component({
  selector: 'app-connect-to-another-wifi',
  templateUrl: './connect-to-another-wifi.component.html',
  styleUrls: ['./connect-to-another-wifi.component.scss']
})
export class ConnectToAnotherWifiComponent implements OnInit {

  @Input()
  public opened: boolean = false;

  @Input()
  connectedWifiName!: string;

  constructor(private alertService: AlertService, private tempService: TempService) {
  }

  ngOnInit(): void {
    this.ssid = this.connectedWifiName;
    this.password = "";
  }

  ssid = "";
  password = "";

  close() {
    this.opened = false;
  }

  connect(){
    if(!this.password.trim() || !this.ssid.trim()) return;

    this.close();
    this.alertService.alert({
      title: "Biztos kapcsolódik a megadott hálózathoz?",
      message: 'Helytelen adatok megadása esetén a termosztátot csak a hozzáférési ponton keresztül lehet majd elérni, ahol meg lehet adni a helyes wifi csatlakozási információkat. Biztosan folytatja?',
      icon: 'warning',
      onOk: () => {
        this.tempService.setWifiCredentials(this.ssid, this.password).then(()=>{
          console.log("Heeeel")
          //window.location.reload();
        });
      },
      showCancelButton: true,
      showOkButton: true,
      isWarning: true,
      okButtonText: "Kapcsolódás",
    })
  }
}
