open Webapi.Dom;

type state = {
  active: option(Types.colors),
  points: int,
  income: int,
};

type action =
  | PlaySound(Types.colors)
  | Click(int)
  | BuyBonus(int, int);

module Styles = {
  open Css;

  global(
    "body",
    [
      fontFamily(
        "-apple-system,BlinkMacSystemFont,\"Segoe UI\",Roboto,Oxygen-Sans,Ubuntu,Cantarell,\"Helvetica Neue\",sans-serif",
      ),
    ],
  );

  let container =
    style([
      display(`flex),
      justifyContent(`center),
      alignItems(`center),
      minHeight(`vh(100.0)),
      flexDirection(`column),
    ]);

  let boxes =
    style([
      display(`flex),
      flexWrap(`wrap),
      maxWidth(`px(500)),
      maxHeight(`px(500)),
    ]);

  let box = (~bgColor: Types.colors, ~active: option(Types.colors)) => {
    let baseStyle = [
      minHeight(`px(120)),
      minWidth(`px(120)),
      border(`px(2), `none, `transparent),
    ];

    let opacity =
      switch (bgColor, active) {
      | (Green, Some(Green)) => opacity(0.5)
      | (Red, Some(Red)) => opacity(0.5)
      | (Blue, Some(Blue)) => opacity(0.5)
      | (Yellow, Some(Yellow)) => opacity(0.5)
      | (_, None) => opacity(1.0)
      | (_, Some(_)) => opacity(1.0)
      };

    let bgColor =
      switch (bgColor) {
      | Green => backgroundColor(`hex("78ab2a"))
      | Red => backgroundColor(`hex("fd2475"))
      | Blue => backgroundColor(`hex("0283c7"))
      | Yellow => backgroundColor(`hex("fde916"))
      };

    style([bgColor, opacity, ...baseStyle]);
  };
  let controls = style([marginTop(`px(10)), textAlign(`center)]);
  let buttons = style([marginTop(`px(10))]);
};

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  initialState: () => {
    active: None,
    points: 0,
    income: 1,
  },
  reducer: (action, state) =>
    switch (action) {
    | PlaySound(color) =>
      ReasonReact.UpdateWithSideEffects(
        {...state, active: Some(color)},
        self => {
          let sound =
            Belt.List.getAssoc(Sounds.map, color, (==))
            ->Belt.Option.getWithDefault(Sounds.green);
          sound##play();
          ();
        },
      )
    | Click(bonus) =>
      ReasonReact.Update({...state, points: state.points + bonus})
    | BuyBonus(bonus, cost) =>
      let {points} = state;
      switch (
        cost <= points
      ) {
        | (true) =>
          ReasonReact.Update({...state, income: state.income + bonus, points: state.points - cost})
        | (false) =>
          ReasonReact.SideEffects(
          (self) => {
            Sounds.error##play();
          },
        )
      }
    },
  render: self => {
    let {active, points, income} = self.state;
    <div className=Styles.container>
      <h1> "ciacho judasza"->ReasonReact.string </h1>
      <h2> {ReasonReact.string(string_of_int(points))} </h2>
      <div className=Styles.boxes>
        <button
          type_="button"
          className={Styles.box(~bgColor=Green, ~active)}
          onClick={_e => self.send(Click(income))}
        />
        <button
          type_="button"
          className={Styles.box(~bgColor=Red, ~active)}
          onClick={_e => self.send(BuyBonus(1,10))}
        />
        <button
          type_="button"
          className={Styles.box(~bgColor=Blue, ~active)}
          //onClick={_e => self.send(Input(Blue))}
        />
        <button
          type_="button"
          className={Styles.box(~bgColor=Yellow, ~active)}
          //onClick={_e => self.send(Input(Yellow))}
        />
      </div>
    </div>;
  },
};

let default = ReasonReact.wrapReasonForJs(~component, _jsProps => make([||]));