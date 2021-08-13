import {Component, Input, OnInit} from '@angular/core';
import {TempService} from '../temp.service';

@Component({
  selector: 'app-navbar',
  templateUrl: './navbar.component.html',
  styleUrls: ['./navbar.component.scss']
})
export class NavbarComponent implements OnInit {

  currentTemp = 23.5;
  targetTemp = 25.0;
  isHeating = true;

  @Input()
  activeTab!: number;

  constructor(private intervalService: TempService) {

  }

  ngOnInit(): void {
  }

}
