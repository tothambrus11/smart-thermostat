import { Component, OnInit } from '@angular/core';
import {TempService} from '../temp.service';
import {ActivatedRoute, Router} from '@angular/router';

@Component({
  selector: 'app-login',
  templateUrl: './login.component.html',
  styleUrls: ['./login.component.scss']
})
export class LoginComponent implements OnInit {


  username: string = "";
  password: string = "";

  constructor(private tempService: TempService, private router: Router, private route: ActivatedRoute) { }

  ngOnInit(): void {
  }

  onSubmit() {
    if(!this.username || !this.password){
      return;
    }

    localStorage.setItem("username", this.username);
    localStorage.setItem("password", this.password);
    this.router.navigate([this.route.snapshot.queryParams.returnUrl || "/intervals"]);
    console.log(this.username, this.password)
  }
}
