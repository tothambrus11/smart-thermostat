import {Component, Input, OnInit} from '@angular/core';

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

  constructor() { }

  ngOnInit(): void {
  }

}
