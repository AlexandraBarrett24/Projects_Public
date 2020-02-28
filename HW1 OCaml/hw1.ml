(* Exercise 5.1 *)
let compose_option : (int -> int option) -> (int -> int option) -> (int -> int option)
  =
  fun (f : int -> int option) ->
  fun (g : int -> int option) ->
  fun ( x : int)->
  let t : int option = (g x) in
  match t with
  |None -> None
  |Some p -> (f p)






(* Exercise 5.2 *)
let join_option : int option option -> int option
  = fun (x : int option option) ->
  match x with
  |Some (Some i) -> Some i
  |_ -> None

  ;;
